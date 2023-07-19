#include <apic.hpp>
#include <exos/io.hpp>
#include <exos/logger.hpp>
#include <exos/panic.hpp>

USE(EXOS::Utils);
USE(EXOS::Platform::MultiProcessor);
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
} ProcessorLEFIAPICEntry;

typedef struct {
	MadtEntry header;
	uint8_t ioapicID;
	uint8_t reserved;
	uint32_t ioapicAddress;
	uint32_t intrBase;
} IOEFIAPICEntry;

typedef struct {
	MadtEntry header;
	uint16_t reserved;
	uintptr_t lapicAddr;
} LEFIAPICAddressEntry;

enum {
	ProcessorLEFIAPIC = 0,
	IOEFIAPIC,
	IOEFIAPICIntrruptSource,
	IOEFIAPICNMI,
	LEFIAPICNMI,
	LEFIAPICAddress,
	ProcessorLx2EFIAPIC = 9
};

static uint8_t *ioapicPtr = nullptr;
static uint8_t *lapicPtr = nullptr;

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
writeIOApic(uint32_t reg, uint32_t value) {
	*(uint32_t *)(ioapicPtr) = reg;
	*(uint32_t *)(ioapicPtr + 0x10) = value;
}

uint32_t
readIOApic(uint32_t reg) {
	*(uint32_t *)(ioapicPtr) = reg;
	return *(uint32_t *)(ioapicPtr + 0x10);
}

void
sendLApicIpi(uint32_t dest, uint32_t value) {
	writeLApic(0x310, dest << 24);
	writeLApic(0x300, value);
	while(readLApic(0x300) & BIT(12)) ASM("pause");
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
		case ProcessorLEFIAPIC: {
			ProcessorLEFIAPICEntry *e = (ProcessorLEFIAPICEntry *)entry;
			if(checkFlag(e->flags, BIT(0))) {
				if(*coreList) {
					(*coreList)[coreIndex++].coreApicId = e->apicID;
					Logger::log(Logger::INFO, "new core detected, lapic id is @", e->apicID);
				} else
					++coreCount;
			}
		} break;
		case IOEFIAPIC: {
			if(!*coreList) continue;
			IOEFIAPICEntry *e = (IOEFIAPICEntry *)entry;
			ioapicPtr = (uint8_t *)(uintptr_t)e->ioapicAddress;
		} break;
		case LEFIAPICAddress: {
			if(!*coreList) continue;
			LEFIAPICAddressEntry *e = (LEFIAPICAddressEntry *)entry;
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
initialize(Acpi::Madt *madt, Core **coreList) {
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
		Logger::log(Logger::INFO, "processor supports x2Apic");
	}

	ASM("movq $0x1b, %%rcx\n\t"
			"rdmsr\n\t"
			"orq %1, %%rax\n\t"
			"wrmsr\n\t"
			"rdmsr"
			: "=a"(eax)
			: "r"(BIT(11) | (ecx & BIT(21) ? BIT(10) : 0)));	//	支持x2apic才开 以前在不支持x2apic的机子上面开卡死了

	if(eax & BIT(11)) {
		Logger::log(Logger::INFO, "apic enabled");
	} else {
		panic("can not enable apic");
	}

	if(eax & BIT(10)) {
		Logger::log(Logger::INFO, "x2apic enabled");
	}

	return coreCount;
}

__NAMESPACE_END