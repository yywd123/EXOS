/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-4-1
*/

#include <OSBase.h>
uint64_t KernelMain(void);

void KernelInit(unsigned long addr)
{
  if(addr & 7) io_hlt();
  struct multiboot_tag *tag;
  unsigned size = *(unsigned *) addr;
  uint32_t color=0xff00ff00;
  unsigned mem_lower;
  unsigned mem_upper;
  for(tag = (struct multiboot_tag *)(addr + 8);
      tag->type != MULTIBOOT_TAG_TYPE_END;
      tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
  {
    switch(tag->type)
    {
      case MULTIBOOT_TAG_TYPE_CMDLINE:
        {}
        break;
      case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        {}
        break;
      case MULTIBOOT_TAG_TYPE_MODULE:
        {}
        break;
      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
          mem_lower = ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower;
          mem_upper = ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper;
        }
        break;
      case MULTIBOOT_TAG_TYPE_BOOTDEV:
        {}
        break;
      case MULTIBOOT_TAG_TYPE_MMAP:
        {}
        break;
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
          Vinfo.bpp = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_bpp;
          Vinfo.pitch = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_pitch;
          Vinfo.Scrn_width = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_width;
          Vinfo.Scrn_height = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_height;
          Vinfo.fb = (void *)(unsigned long)((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_addr;
          DrawBlock(0, 0, ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_height, 
              ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_width, 0xff80ccff);                   //Background
          DrawBlock(0, 0, 24, ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_width, 0xff2f4f4f);   //TaskBar
        }    
        break;
    }
  }
  uint64_t Stat = KernelMain();
  if(Stat <= 0 | Stat >= 5)     //Error
  {
    DrawBlock(0, 0, ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_height, 
              ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_width, 0xff0044ff);
    io_hlt();
  }
  io_hlt();
}


uint64_t KernelMain(void)
{
  uint64_t SYSStat = 0x0;
  WINDOW info;
  info.x=20;
  info.y=20;
  info.h=25;
  info.v=80;
  info.WindowType=0x00;
 
  CreateWindow(info); 
  
  return SYSStat;
}
