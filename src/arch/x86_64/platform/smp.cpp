#include <platform.hpp>
#include <efi/efi.hpp>
#include <exos/acpi.hpp>
#include <exos/logger.hpp>
#include <apic.hpp>
#include <exos/panic.hpp>

USE(EXOS::Drivers);
USE(EXOS::Utils);

extern "C" void
testHandler() {
	Logger::log(Logger::INFO, "testHandler called");
	ASM("hlt");
	while(1)
		;
}

template<typename T>
struct _DescriptorTable {
	uint16_t size;
	Ptr<T> descriptor;

	_DescriptorTable(Ptr<T> desc, uint16_t descCount) :
		size(sizeof(T) * descCount - 1), descriptor(desc) {
	}
} __packed;

template<typename T>
using DescriptorTable = struct _DescriptorTable<T>;

__NAMESPACE_DECL(Platform::MultiProcessor)

static Core *coreList = nullptr;
static uint8_t coreCount = 0;

static SegmentDescriptor gdt[16];

static inline void
setSegmentDescriptor(uint8_t index, unsigned privilege, unsigned flag) {
	SegmentDescriptor *entry = &gdt[index];
	entry->isVaild = true;
	entry->isLongModeCode = checkFlag(flag, BIT(3));
	entry->descType = 1;
	entry->privilege = privilege;
	entry->flag = flag;
}

static inline void
setSystemSegmentDescriptor(uint8_t index, void *base, uint32_t limit, uint8_t flag) {
	SystemSegmentDescriptor *entry = (SystemSegmentDescriptor *)&gdt[4 + index * 2];
	entry->isVaild = true;
	entry->descType = 0;
	entry->privilege = 0;
	entry->flag = flag;
	entry->limit0 = (limit & 0xffff);
	entry->limit1 = (limit >> 16) & 0b1111;
	entry->base0 = ((uintptr_t)base & 0xffff);
	entry->base1 = ((uintptr_t)base >> 16) & 0xff;
	entry->base2 = ((uintptr_t)base >> 24) & 0xff;
	entry->base3 = ((uintptr_t)base >> 32);
}

static uint8_t
getCurrentCoreApicID() {
	uint32_t apicID = 0;
	uint32_t tmp;
	__cpuid(0x1, tmp, apicID, tmp, tmp);
	return (apicID >> 24);
}

static void
setCurrentCoreGS(uintptr_t value) {
	ASM("movq $0xc0000101, %%rcx\n\t"
			"wrmsr\n\t"
			"movq $0xc0000102, %%rcx\n\t"
			"wrmsr"
			:
			: "edx"((value >> 32)),
				"eax"((value & 0xffffffff)));
}

static uint8_t
getCurrentCoreIndex() {
	uint8_t currentApicId = getCurrentCoreApicID();
	__iter(coreCount) {
		if(coreList[i].coreApicId == currentApicId) return i;
	}

	panic("can not get current core index! It should NOT happens!");
}

static void
dumpCoreList() {
	Logger::log(Logger::INFO, "@ cores detected", (int16_t)coreCount);
	__iter(coreCount) {
		Logger::log(Logger::INFO, "core @:\n\tapic id = @\n\tidt @",
								i, coreList[i].coreApicId, coreList[i].idt);
	}
}

void __INIT
initialize() {
	//  初始化acpi与apic
	EfiSystemTable *systemTable = efiGetSystemTable();

	void *acpiRsdptr = nullptr;
	void *acpi2Rsdptr = nullptr;

#define EntryMatcher(_guid, _table)                                 \
	if(guidCompare(systemTable->ConfigurationTable[i].guid, _guid)) { \
		_table = systemTable->ConfigurationTable[i].table;              \
		continue;                                                       \
	}
	__iter(systemTable->tableEntryCount) {
		EntryMatcher("eb9d2d30-2d88-11d3-9a16-0090273fc14d", acpiRsdptr);
		EntryMatcher("8868e871-e4f1-11d3-bc22-0080c73c8881", acpi2Rsdptr);
	}
#undef EntryMatcher

	if(acpi2Rsdptr) acpiRsdptr = acpi2Rsdptr;
	Acpi::initialize((Acpi::Rsdp *)acpiRsdptr);

	coreCount = Apic::initialize(Acpi::getTable<Acpi::Madt>("APIC"), &coreList);

	dumpCoreList();

	// setCurrentCoreGS(getCurrentCoreIndex());

	//  初始化bsp的gdt与idt
	// setSegmentDescriptor(1, 0, BIT(3));
	// setSegmentDescriptor(2, 0, BIT(1));

	// uintptr_t fn = 0;
	// getAddressFromSymbol(fn, "testHandler");
	// __iter(0x20) {
	// 	coreList[0].idt[i].set(fn, 8, 0, 0x8f);
	// }

	// ASM("lgdt %0" ::"m"(DescriptorTable<SegmentDescriptor>(gdt, 16)));
	// ASM("mov $0, %rax\n\t"	//  数据段选择子(2 << 3)
	// 		"mov %rax, %ds\n\t"
	// 		"mov %rax, %es\n\t"
	// 		"mov %rax, %fs\n\t"
	// 		"mov %rax, %gs\n\t"
	// 		"mov $0, %rax\n\t"
	// 		"mov %rax, %ss\n\t"
	// 		"pushq $8\n\t"	//  代码段选择子(1 << 3)
	// 		"lea 3(%rip), %rax\n\t"
	// 		"pushq %rax\n\t"
	// 		"lretq");

	// ASM("lidt %0" ::"m"(DescriptorTable<InterruptDescriptor>(coreList[0].idt, 256)));
}

__NAMESPACE_END