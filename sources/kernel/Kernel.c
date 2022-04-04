/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-4-1
*/

#include <OSBase.h>

void io_hlt();

int KernelInit(unsigned long magic, unsigned long addr);

int KernelInit(unsigned long magic, unsigned long addr)
{
  if(magic != 0xe85250d6 || addr & 7) return -1;
  struct multiboot_tag *tag;
  unsigned size = *(unsigned *) addr;
  unsigned color;
  unsigned mem_lower;
  unsigned mem_upper;
  struct multiboot_tag_framebuffer *tagfb;
  void *fb;
  for(tag = (struct multiboot_tag *)(addr + 8);
      tag->type != 0;
      tag = (struct multiboot_tag *)(uint8_t *)tag + ((tag->size + 7) & ~ 7))
  {
    switch(tag->type)
    {
      case 4:
        mem_lower = ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower;
        mem_upper = ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper;
        break;
      case 8:
        tagfb = (struct multiboot_tag_framebuffer *)tag;
        for(int i = 0; i > 1024; ++i)
        DrawPixel(tagfb, i, i-1, 0xffffffff);
    }
  }
}
