/*  System Init Part for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-24
*/

#include <OSBase.h>

unsigned long Bootinfo_addr;

extern BootInfo BOOTINFO;
extern VideoInfo Vinfo; 

void EXOSAPI KernelInit(void)
{
  if(Bootinfo_addr & 7) io_hlt();
  struct multiboot_tag *tag;
  for(tag = (struct multiboot_tag *)(Bootinfo_addr + 8);
      tag->type != MULTIBOOT_TAG_TYPE_END;
      tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
  {
    switch(tag->type)
    {
      case MULTIBOOT_TAG_TYPE_MODULE:
        {
          BOOTINFO.ModInfo = (struct multiboot_tag_module *)tag;
          //IsModuleExist = true;
        }
        break;
      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
          BOOTINFO.mem_upper = ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper;
          BOOTINFO.mem_lower = ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower;
          //IsMemInfoExist = true;
        }
        break;
      case MULTIBOOT_TAG_TYPE_MMAP:
        {
          BOOTINFO.MemMap = (struct multiboot_tag_mmap *)tag;
          //IsMemMapInfoExist = true;
        }
        break;
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
          Vinfo.bpp = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_bpp;
          Vinfo.pitch = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_pitch;
          Vinfo.Scrn_width = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_width;
          Vinfo.Scrn_height = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_height;
          Vinfo.BackGround_Color = 0xff0080bb;
          Vinfo.ForeGround_Color = 0xffffffff;
          Vinfo.fb = (void *)(unsigned long)((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_addr;
          Vinfo.Cursor_x = 0;
          Vinfo.Cursor_y = 0;

          DrawBlock(0, 0, ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_height, 
              ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_width, Vinfo.BackGround_Color);                 //Background
        }    
        break;
    }
  }
  InitSerialPort(COM1);

  printk(LOG_INFO, "Kernel Init Success!!");
  puts(L"[ INFO ] \x7cfb\x7edf\x521d\x59cb\x5316\x6210\x529f!!\n");
}

