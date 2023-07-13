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
			if(checkFlag(e->flags, BIT(0)) && *coreList) {
				(*coreList)[coreCount].coreApicId = e->apicID;
				Logger::log(Logger::INFO, "new core detected, lapic id is @", e->apicID);
			}
			++coreCount;
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
initialize(Acpi::Madt *madt, Core **coreList) {
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
			"orq $0xc00, %%rax\n\t"	 //	尝试一次性同时启用apic和x2Apic(启用失败的会自动回弹)
			"wrmsr\n\t"
			"rdmsr"
			: "=a"(eax));

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