#include <platform/platform.hpp>
#include <exos/logger.hpp>
#include <exos/panic.hpp>

USE(EXOS::Utils);
USE(EXOS::Platform::Processor);
USE(EXOS::Platform::IO);

typedef struct {
	uint8_t type;
	uint8_t length;
} MadtEntry;

typedef struct {
	MadtEntry header;
	uint8_t apicProcessorID;
	uint8_t apicID;
	uint32_t flags;
} ProcessorLAPICEntry;

typedef struct {
	MadtEntry header;
	uint8_t ioapicID;
	uint8_t reserved;
	uint32_t ioapicAddress;
	uint32_t intrBase;
} IOAPICEntry;

typedef struct {
	MadtEntry header;
	uint16_t reserved;
	uintptr_t lapicAddr;
} LAPICAddressEntry;

enum {
	ProcessorLAPIC = 0,
	IOAPIC,
	IOAPICIntrruptSource,
	IOAPICNMI,
	LAPICNMI,
	LAPICAddress,
	ProcessorLx2APIC = 9
};

static uint8_t *ioapicPtr = nullptr;
static uint8_t *lapicPtr = nullptr;

static bool x2ApicEnabled = false;

__NAMESPACE_DECL(Drivers::Apic)

void
writeLApic(uint32_t reg, uint32_t value) {
	*(uint32_t *)(lapicPtr + reg) = value;
}

uint32_t
readLApic(uint32_t reg) {
	return *(uint32_t *)(lapicPtr + reg);
}

void
writeLApicLVT(LVTIndex index, uint32_t value) {
	writeLApic(0x2f0 + index * 0x10, value);
}

uint32_t
readLApicLVT(LVTIndex index) {
	return readLApic(0x2f0 + index * 0x10);
}

void
maskLApicLVT(LVTIndex index, bool enable) {
	uint32_t value = readLApicLVT(index);
	writeLApicLVT(index, enable ? value & ~BIT(16) : value | BIT(16));
}

void
lApicAck() {
	if(x2ApicEnabled) {
		ASM("mov $0x80b, %rcx\n\t"
				"mov $0, %rax\n\t"
				"mov $0, %rdx\n\t"
				"wrmsr");
	} else
		writeLApic(0xb0, 0);
}

void
writeIOApic(uint8_t reg, uint32_t value) {
	*(uint32_t *)(ioapicPtr) = reg;
	*(uint32_t *)(ioapicPtr + 0x10) = value;
}

uint32_t
readIOApic(uint8_t reg) {
	*(uint32_t *)(ioapicPtr) = reg;
	return *(uint32_t *)(ioapicPtr + 0x10);
}

void
writeIOApicRTE(uint8_t index, uint64_t value) {
	index = index * 2 + 0x10;
	writeIOApic(index, value & 0xffffffff);
	writeIOApic(index + 1, value >> 32);
}

uint64_t
readIOApicRTE(uint8_t index) {
	index = index * 2 + 0x10;
	uint64_t value = 0;
	value = readIOApic(index + 1);
	value <<= 32;
	value |= readIOApic(index);

	return value;
}

void
registerIOApicRTE(uint8_t index, uint64_t *entry) {
	writeIOApicRTE(index, *entry);
}

void
unregisterIOApicRTE(uint8_t index) {
	writeIOApicRTE(index, BIT(16));
}

void
maskIOApicRTE(uint8_t index, bool enable) {
	uint64_t value = readIOApicRTE(index);
	writeIOApicRTE(index, enable ? value & ~BIT(16) : value | BIT(16));
}

void
ioApicEdgeAck() {
	lApicAck();
}

void
ioApicLevelAck(uint8_t irq) {
	ioApicEdgeAck();
	writeIOApic(0xb0, irq);
}

void
sendLApicIpi(uint32_t dest, uint32_t value) {
	writeLApic(0x310, dest << 24);
	writeLApic(0x300, value);
	while(readLApic(0x300) & BIT(12)) Logger::print('.');
}

static uint8_t __INIT
parseMadt(Acpi::Madt *madt, Core **coreList) {
	if(!coreList) {
		panic("coreList is null");
	}
	uint8_t coreCount = 0;
	uint8_t coreIndex = 0;
	lapicPtr = (uint8_t *)(uintptr_t)madt->lapicAddr;
parse:
	uint8_t *p = madt->data;
	uint32_t length = madt->header.length - sizeof(Acpi::Madt);
	while(length != 0) {
		MadtEntry *entry = (MadtEntry *)p;
		p += entry->length;
		length -= entry->length;
		switch(entry->type) {
		case ProcessorLAPIC: {
			ProcessorLAPICEntry *e = (ProcessorLAPICEntry *)entry;
			if(checkFlag(e->flags, BIT(0))) {
				if(*coreList) {
					(*coreList)[coreIndex++].coreApicId = e->apicID;
				} else
					++coreCount;
			}
		} break;
		case IOAPIC: {
			if(!*coreList) continue;
			IOAPICEntry *e = (IOAPICEntry *)entry;
			ioapicPtr = (uint8_t *)(uintptr_t)e->ioapicAddress;
		} break;
		case LAPICAddress: {
			if(!*coreList) continue;
			LAPICAddressEntry *e = (LAPICAddressEntry *)entry;
			lapicPtr = (uint8_t *)e->lapicAddr;
		} break;
		default: continue;
		}
	}
	if(*coreList) return coreCount;

	*coreList = new Core[coreCount];
	goto parse;
}

uint8_t __INIT
initialize(Core **coreList) {
	auto madt = Acpi::getTable<Acpi::Madt>("APIC");
	if(!madt) {	 //	你这b电脑什么回事 都支持efi了 还不支持apic是吧
		panic("acpi madt not found");
	}

	uint8_t coreCount = parseMadt(madt, coreList);

	//  禁用PIC
	outb(0x21, 0xff);
	outb(0xa1, 0xff);

	//  apic,启动!
	uint32_t tmp;
	uint32_t eax, ecx, edx;
	__cpuid(1, eax, tmp, ecx, edx);
	if(!(edx & BIT(9))) {
		panic("processor doesn't support Apic");
	}

	if(ecx & BIT(21)) {
		Logger::log(Logger::DEBUG, "processor supports x2Apic");
	}

	lvt_iter({
		maskLApicLVT(lvt, false);
	});

	ASM("movq $0x1b, %%rcx\n\t"
			"rdmsr\n\t"
			"orq %1, %%rax\n\t"
			"wrmsr\n\t"
			"rdmsr"
			: "=a"(eax)
			: "r"(BIT(8) | BIT(11) | (ecx & BIT(21) ? BIT(10) : 0)));	 //	支持x2apic才开 以前在不支持x2apic的机子上面开卡死了

	writeLApic(0xf0, readLApic(0xf0) | BIT(8));
	uint32_t svr = readLApic(0xf0);

	if(eax & BIT(11) && svr & BIT(8)) {
		Logger::log(Logger::DEBUG, "apic enabled");
	} else {
		panic("can not enable apic");
	}

	if(eax & BIT(10)) {
		x2ApicEnabled = true;
		Logger::log(Logger::DEBUG, "x2apic enabled");
	}

	return coreCount;
}

__NAMESPACE_END