#include <platform/platform.hpp>
#include <exos/acpi.hpp>
#include <exos/logger.hpp>
#include <exos/panic.hpp>

USE(EXOS::Drivers);
USE(EXOS::Utils);

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

__NAMESPACE_DECL(Platform::Processor)

static Core *coreList = nullptr;
static uint8_t coreCount = 0;

static SegmentDescriptor gdt[16] = {0};
static InterruptDescriptor idt[256] = {0};
static TaskStateSegment tss = {0};

static bool isCurrentAPinitialized = false;
static uintptr_t currentAPstack = 0;

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
	SystemSegmentDescriptor *entry = (SystemSegmentDescriptor *)&gdt[3 + index * 2];
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

uint8_t
getCurrentCoreApicID() {
	uint32_t apicID = 0;
	uint32_t tmp;
	__cpuid(0x1, tmp, apicID, tmp, tmp);
	return (apicID >> 24);
}

static uint8_t
getCurrentCoreIndex() {
	uint8_t currentApicId = getCurrentCoreApicID();
	_iter(coreCount) {
		if(coreList[i].coreApicId == currentApicId) return i;
	}

	panic("can not get current core index! It should NOT happens!");
	__UNREACHABLE;
}

static void
dumpCoreList() {
	Logger::log(Logger::DEBUG, "smp: detected @ cores from Acpi", (int16_t)coreCount);
	_iter(coreCount) {
		Logger::log(Logger::DEBUG, "core @: apic id = @", i, coreList[i].coreApicId);
	}
}

void __INIT
initialize() {
	//  初始化apic

	coreCount = Apic::initialize(&coreList);

	dumpCoreList();

	setSegmentDescriptor(1, 0, BIT(3));
	setSegmentDescriptor(2, 3, BIT(3));
	setSystemSegmentDescriptor(0, &tss, sizeof(TaskStateSegment), TSS_Available);

	Interrupt::initialize(idt);
	ASM("lgdt %0" ::"m"(DescriptorTable<SegmentDescriptor>(gdt, 16)));
	ASM("mov $0, %rax\n\t"
			"mov %rax, %ds\n\t"
			"mov %rax, %es\n\t"
			"mov %rax, %fs\n\t"
			"mov %rax, %gs\n\t"
			"mov %rax, %ss\n\t"
			"mov $3 << 3, %ax\n\t"
			"ltr %ax\n\t"
			"pushq $1 << 3\n\t"	 //  内核代码段选择子(1 << 3)
			"lea 3(%rip), %rax\n\t"
			"pushq %rax\n\t"
			"lretq");
	ASM("lidt %0" ::"m"(DescriptorTable<InterruptDescriptor>(idt, 256)));

	//	技术力不够 单核将就着用先吧

	// uintptr_t apBootstrapArea = (uintptr_t)KMemory::alloc4KPages(1);
	// Logger::log(Logger::DEBUG, "area @", apBootstrapArea);
	// if(apBootstrapArea == 0) panic("invaild ap bootstrap area");

	// uintptr_t apBootstrap_start = 0;
	// uintptr_t apBootstrap_end = 0;
	// getAddressFromSymbol(apBootstrap_start, "_apBootstrap_start");
	// getAddressFromSymbol(apBootstrap_end, "_apBootstrap_end");
	// Logger::log(Logger::DEBUG, "ap start: @, end: @", apBootstrap_start, apBootstrap_end);

	// DescriptorTable<SegmentDescriptor> *apBootstrap_gdt = nullptr;
	// uint32_t *apBootstrap_pm = nullptr;
	// getAddressFromSymbol(apBootstrap_gdt, "_apBootstrap_gdt");
	// getAddressFromSymbol(apBootstrap_pm, "_apBootstrap_pm");
	// Logger::log(Logger::DEBUG, "gdt @, pm @", apBootstrap_gdt, apBootstrap_pm);
	// *apBootstrap_gdt = DescriptorTable<SegmentDescriptor>(gdt, 16);
	// *apBootstrap_pm = (uint32_t)(uintptr_t)Paging::getPageLevelMax();

	// __builtin_memcpy((void *)apBootstrapArea, (void *)apBootstrap_start, apBootstrap_end - apBootstrap_start);
	// // Logger::log(Logger::DEBUG, "cmp @", __builtin_memcmp((void *)apBootstrapArea, (void *)apBootstrap_start, apBootstrap_end - apBootstrap_start));

	// for(uint8_t i = 1; i < coreCount; ++i) {
	// 	isCurrentAPinitialized = false;

	// 	currentAPstack = (uintptr_t)KMemory::alloc4KPages(8);
	// 	currentAPstack += 8 * PAGE_4K;

	// 	Logger::log(Logger::DEBUG, "sending ipi to apic id: @", coreList[i].coreApicId);

	// 	Apic::sendLApicIpi(coreList[i].coreApicId, 0x4500);
	// 	Logger::log(Logger::DEBUG, "INIT sent");
	// 	Hpet::sleep(10, TimeUnit::MILLISECONDS);

	// 	__iter(j, 2) {
	// 		Apic::sendLApicIpi(coreList[i].coreApicId, 0x4600 + (apBootstrapArea >> PAGE_4KSHIFT));
	// 		Logger::log(Logger::DEBUG, "SIPI sent");
	// 		Hpet::sleep(10, TimeUnit::MILLISECONDS);
	// 		if(isCurrentAPinitialized) break;
	// 	}

	// 	while(!isCurrentAPinitialized) ASM("pause");
	// }
}

// extern "C" void __INIT
// apInitialize() {
// 	ASM("mov %0, %%rsp" ::"r"(currentAPstack));

// 	uint8_t coreIndex = getCurrentCoreIndex();
// 	setCurrentCoreGS(coreIndex);

// 	Logger::log(Logger::DEBUG, "I AM CORE @", (int16_t)coreIndex);

// 	isCurrentAPinitialized = true;

// 	while(1)
// 		;
// }

__NAMESPACE_END