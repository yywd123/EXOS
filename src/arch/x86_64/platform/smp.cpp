#include <efi/efi.h>
#include <platform.hpp>
#include <exos/fbcon.hpp>

#include <exos/serial.hpp>
USE(EXOS::Drivers);

extern "C" void
testHandler() {
  Serial::write(Serial::COM1, '!');
  ASM("hlt");
  while (1);
}

template<typename T>
struct _DescriptorTable {
  uint16_t size;
  Ptr<T> descriptor;

  _DescriptorTable(Ptr<T> desc, uint16_t descCount) : size(sizeof(T) * descCount - 1), descriptor(desc) {}
} __packed;

template<typename T>
using DescriptorTable = struct _DescriptorTable<T>;

__NAMESPACE_DECL(Platform::MultiProcessor)

static SMPCore *apCoreList = nullptr;
static uint8_t apCoreCount = 0;

static SMPCore bspCore;

uint32_t
currentProcessorApicId() {
  uint32_t eax, edx;
  ASM("rdmsr":"=a" (eax),"=d" (edx):"c"(0x1b));
  return (edx << 24) | (eax >> 8);
}

static inline void printUInt(uint64_t i, uint8_t n) {  //  统一打印十六进制
  if (i == 0) {
    for(int i = 0; i < n; ++i) efiPuts(L"0");
    return;
  } 
  const char *digits = "0123456789abcdef";
  char buf[17] = {0};
  for (uint8_t j = 16; j != 0; --j) {
    buf[j - 1] = digits[i & 0xf];
    i >>= 4;
  }

  const char *p = &buf[16 - n];

  FbConsole::print(p);
}

SegmentDescriptor gdt[16];

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
  SystemSegmentDescriptor *entry = (SystemSegmentDescriptor*)&gdt[4 + index * 2];
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

typedef struct {
  uint16_t size;
  uintptr_t address;
} __packed DescTable;

void __INIT
initBootstrapProcessor() {
  setSegmentDescriptor(1, 0, BIT(3));
  setSegmentDescriptor(2, 0, BIT(1));

  bspCore.coreApicId = currentProcessorApicId();

  printUInt(bspCore.coreApicId, 8);

  uintptr_t fn = 0;
  getAddressFromSymbol(fn, "testHandler");
  __iter(0x20) {
    bspCore.idt[i].set(fn, 8, 0, 0x8f);
    Serial::write(Serial::COM1, '.');
  }

  Serial::write(Serial::COM1, '=');
  ASM("lgdt %0"::"m"(DescriptorTable<SegmentDescriptor>(gdt, 16)));
  ASM(
    "mov $0, %rax\n\t"  //  数据段选择子(2 << 3)
    "mov %rax, %ds\n\t"
    "mov %rax, %es\n\t"
    "mov %rax, %fs\n\t"
    "mov %rax, %gs\n\t"
    "mov $0, %rax\n\t"
    "mov %rax, %ss\n\t"
    "pushq $8\n\t"       //  代码段选择子(1 << 3)
    "lea 3(%rip), %rax\n\t"
    "pushq %rax\n\t"
    "lretq");

  ASM("lidt %0"::"m"(DescriptorTable<InterruptDescriptor>(bspCore.idt, 256)));
  Serial::write(Serial::COM1, '=');

  Serial::write(Serial::COM1, '=');
}

void __INIT
initApplicationProcessors() {

}

__NAMESPACE_END