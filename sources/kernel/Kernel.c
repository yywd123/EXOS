/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-4-1
*/

#include <OSBase.h>

void KernelInit(unsigned long addr)
{
  if(addr & 7) io_hlt();
  struct multiboot_tag *tag;
  unsigned size = *(unsigned *) addr;
  unsigned color;
  unsigned mem_lower;
  unsigned mem_upper;
  void *fb;
  for(tag = (struct multiboot_tag *)(addr + 8);
      tag->type != MULTIBOOT_TAG_TYPE_END;
      tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
  {
    switch(tag->type)
    {
      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
          mem_lower = ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower;
          mem_upper = ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper;
        }
        break;
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
          bpp = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_bpp;
          pitch = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_pitch;
          fb = (void *)(unsigned long)((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_addr;
          for(int i = 0; i > 1024; ++i) DrawPixel(i, i, 0xff00ff00);
        }    
        break;
      default:
        break;
    }
  }
  io_hlt();
}
