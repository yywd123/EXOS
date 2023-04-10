#include <arch/arch.h>

typedef struct {
  uint16_t size;
  uintptr_t address;
} __attribute__((packed)) DescTable;

typedef struct {
  uint32_t unused0;
  uint8_t unused1;
  unsigned flag : 4;
  unsigned descType : 1;
  unsigned privilege : 2;
  bool isVaild : 1;
  unsigned unused2 : 4;
  unsigned reserved : 1;
  bool isLongModeCode : 1;
  unsigned unused3 : 2;
  uint8_t unused4;
} __attribute__((packed)) SegmentDesc;

typedef struct {
  uint16_t limit0;
  uint16_t base0;
  uint8_t base1;
  unsigned flag : 4;
  unsigned descType : 1;
  unsigned privilege : 2;
  bool isVaild : 1;
  unsigned limit1 : 4;
  unsigned usused : 4;
  uint8_t base2;
  uint32_t base3;
  uint32_t reserved;
} __attribute__((packed)) SSDesc;

typedef struct {
  uint16_t offset0;        // offset bits 0..15
  uint16_t selector;       // a code segment selector in GDT or LDT
  uint8_t  ist;            // bits 0..2 holds GateDesc Stack Table offset, rest of bits zero.
  uint8_t  attributes;     // gate type, dpl, and p fields
  uint16_t offset1;        // offset bits 16..31
  uint32_t offset2;        // offset bits 32..63
  uint32_t reserved;
} __attribute__((packed)) GateDesc;

enum {
  System = 0,
  Normal = 1
};

static SegmentDesc gdt[64];
static GateDesc idt[256];

void makeGDTEntry(uint8_t index, unsigned privilege, unsigned flag) {
  SegmentDesc *entry = &gdt[index];
  entry->isVaild = true;
  entry->isLongModeCode = checkFlag(flag, SEGFLAG_EXCUTEABLE);
  entry->descType = Normal;
  entry->privilege = privilege;
  entry->flag = flag;
}

void makeGDTSSEntry(uint8_t index, void *base, uint32_t limit, uint8_t flag) {
  SSDesc *entry = (SSDesc*)&gdt[9 + index * 2];
  entry->isVaild = true;
  entry->descType = System;
  entry->privilege = 0;
  entry->flag = flag;
  entry->limit0 = (limit & 0xffff);
  entry->limit1 = (limit >> 16) & 0b1111;
  entry->base0 = ((uintptr_t)base & 0xffff);
  entry->base1 = ((uintptr_t)base >> 16) & 0xff;
  entry->base2 = ((uintptr_t)base >> 24) & 0xff;
  entry->base3 = ((uintptr_t)base >> 32);
}

void registerInterrupt(uint8_t index, uintptr_t entryPoint, uint16_t selector, uint8_t ist, IntrType type) {
  GateDesc *entry = &idt[index];
  entry->offset0 = (entryPoint & 0xffff);
  entry->selector = selector;
  entry->ist = (ist & 7);
  entry->attributes = (type & 0xff);
  entry->offset1 = ((entryPoint >> 16) & 0xffff);
  entry->offset2 = ((entryPoint >> 32) & 0xffffffff);
}

DescTable gdtr = {
  .size = sizeof(gdt) - 1,
  .address = (uintptr_t)&gdt[0]
};
DescTable idtr = {
  .size = sizeof(idt) - 1,
  .address = (uintptr_t)&idt[0]
};

void loadDT() {
  ASM("lgdt gdtr(%rip)\n\t"
      "mov $0, %rax\n\t"  //  数据段选择子(2 << 3)
      "mov %rax, %ds\n\t"
      "mov %rax, %es\n\t"
      "mov %rax, %fs\n\t"
      "mov %rax, %gs\n\t"
      "mov $0, %rax\n\t"
      "mov %rax, %ss\n\t"
      "pushq $0b1000\n\t"       //  代码段选择子(1 << 3)
      "lea 1f(%rip), %rax\n\t"
      "pushq %rax\n\t"
      "lretq\n1:");
  ASM("lidt idtr(%rip)");
}