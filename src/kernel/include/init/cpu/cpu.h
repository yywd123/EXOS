/*  CPU Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-20
*/

typedef struct{
  uint16_t limit_low, base_low;
  uint8_t base_mid, access_right;
  uint8_t limit_high, base_high;
}__attribute__((packed)) SEGDESC;

typedef struct{
  uint16_t offset_low;
  uint16_t segment;
  unsigned ist : 3, zero0 : 5, dpl : 2, p : 1;
  uint16_t offset_middle;
  uint32_t offset_high;
  uint32_t zero1;
}__attribute__((packed)) GATEDESC;

#define PIC0_ICW1 0x20
#define PIC0_OCW2 0x20
#define PIC0_IMR 0x21
#define PIC0_ICW2 0x21
#define PIC0_ICW3 0x21
#define PIC0_ICW4 0x21
#define PIC1_ICW1 0xa0
#define PIC1_OCW2 0xa0
#define PIC1_IMR 0xa1
#define PIC1_ICW2 0xa1
#define PIC1_ICW3 0xa1
#define PIC1_ICW4 0xa1

void SEGDESC_Init(SEGDESC *gdt, uint32_t limit, uint32_t base, uint32_t access)
{
  if(limit > 0xfffff)
  {
    access |= 0x8000;
    limit /= 0x1000;
  }
  gdt->limit_low = limit & 0xffff;
  gdt->base_low = base & 0xffff;
  gdt->base_mid = (base >> 16) & 0xff;
  gdt->access_right = access & 0xff;
  gdt->limit_high = ((limit >> 16) & 0x0f) | ((access >> 8) & 0xf0);
  gdt->base_high = (base >> 24) & 0xff;
  return;
}

void GATEDESC_Init(GATEDESC *idt, uint32_t offset, uint32_t selector, uint32_t access)
{
  idt->offset_low = offset & 0xffff;
  idt->selector = selector;
  idt->dwCount = (access >> 8) & 0xff;
  idt->access_right = access & 0xff;
  idt->offset_high = (offset >> 16) & 0xffff;
  return;
}

void EXOSAPI PIC_Init(void)
{
  outb(PIC0_IMR, 0xff);
  outb(PIC1_IMR, 0xff);
  outb(PIC0_ICW1, 0x11);
  outb(PIC0_ICW2, 0x20);
  outb(PIC0_ICW3, 1 << 2);
  outb(PIC0_ICW4, 0x01);

  outb(PIC1_ICW1, 0x11);
  outb(PIC1_ICW2, 0x28);
  outb(PIC1_ICW3, 0x02);
  outb(PIC1_ICW4, 0x01);

  outb(PIC0_IMR, 0xfb);
  outb(PIC1_IMR, 0xff);
  return;
}

void EXOSAPI GDTIDT_Init(void)
{
  SEGDESC *gdt = (SEGDESC *) 0x00270000;
  GATEDESC *idt = (GATEDESC *) 0x0026f800;
  uint32_t i;

  // Init GDT
  for(i = 0; i < 8192; ++i) SEGDESC_Init(gdt + i, 0, 0, 0);
  SEGDESC_Init(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
  SEGDESC_Init(gdt + 2, 0x0007ffff, 0x00800000, 0x409a);
  load_gdtr(0xffff, 0x27000000);

  // Init IDT
  for(i = 0; i < 256; ++i) GATEDESC_Init(idt + i, 0, 0, 0);
  load_idtr(0x07ff, 0x0026f800);

  // Init Interrupt
  GATEDESC_Init(idt + 0x21, (int) KB_INTHandler, 16, 0x8e);   //Keyboard

  return;
}
