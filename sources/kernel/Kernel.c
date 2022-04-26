/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-4-1
*/

#include <OSBase.h>

bool IsCmdLineExist = false;
bool IsLoaderNameExist = false;
bool IsModuleExist = false;
bool IsMemInfoExist = false;
bool IsMemMapInfoExist = false;

unsigned long addr;

void EXOSAPI KernelInit(void)
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
        {
          BOOTINFO.CmdLine = ((struct multiboot_tag_string *)tag)->string;
          IsCmdLineExist = true;
        }
        break;
      case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        {
          BOOTINFO.LoaderName = ((struct multiboot_tag_string *)tag)->string;
          IsLoaderNameExist = true;
        }
        break;
      case MULTIBOOT_TAG_TYPE_MODULE:
        {
          BOOTINFO.ModInfo = (struct multiboot_tag_module *)tag;
          IsModuleExist = true;
        }
        break;
      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
          mem_lower = ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower;
          mem_upper = ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper;
          IsMemInfoExist = true;
        }
        break;
      case MULTIBOOT_TAG_TYPE_MMAP:
        {
          BOOTINFO.MemMap = (struct multiboot_tag_mmap *)tag;
          IsMemMapInfoExist = true;
        }
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
}


void EXOSAPI KernelMain(void)
{
  KernelInit();
  uint64_t SYSStat = 1;
  WINDOW info;
  info.x=20;
  info.y=20;
  info.h=25;
  info.v=80;
  info.WindowType=0x00;
 
  CreateWindow(info); 

  InitSerialPort(COM1);
  WriteSerialPort('A', COM1);
  info.x=80;
  info.y=80;
  info.h=100;
  info.v=170;
  info.WindowType=0x01;
 
  CreateWindow(info);

  switch(SYSStat)
  {
    case 0:
      Shutdown();
      break;
    case 1:
      Reboot();
      break;
    default:
      {
      DrawBlock(0, 0, ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_height, 
                ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_width, 0xff0044ff);
      io_hlt();
      }
      break;
  }
}
