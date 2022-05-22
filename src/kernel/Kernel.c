/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-5-4
*/

// KernelBase Library
#include <OSBase.h>
#include <io/io.h>

// KernelFunc Library
#include <debug/debug.h>

bool IsCmdLineExist = false;
bool IsLoaderNameExist = false;
bool IsModuleExist = false;
bool IsMemInfoExist = false;
bool IsMemMapInfoExist = false;

unsigned long addr;
BootInfo BOOTINFO;
Video_Info Vinfo;

#include <EXOS/expection.h>
//#include <EXOS/cpu/cpu.h>
//#include <EXOS/irq.h>

uint64_t EXOSAPI SYSMain(void)
{ 
  uint64_t SYSStat = 0;
  puts(L"Welcome to EXOS 0.0.1(build date:2022-5-21)\n");
  puts(L"root:/ $>");
  return SYSStat;
}

void EXOSAPI KernelInit(void)
{
  if(addr & 7) io_hlt();
  struct multiboot_tag *tag;
  for(tag = (struct multiboot_tag *)(addr + 8);
      tag->type != MULTIBOOT_TAG_TYPE_END;
      tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
  {
    switch(tag->type)
    {
/*      case MULTIBOOT_TAG_TYPE_CMDLINE:
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
        break;*/
      case MULTIBOOT_TAG_TYPE_MODULE:
        {
          BOOTINFO.ModInfo = (struct multiboot_tag_module *)tag;
          IsModuleExist = true;
        }
        break;
      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
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
}


void EXOSAPI KernelMain(void)
{
  KernelInit();

  puts(L"EXOS v0.1a \x4f5c\x8005:yywd_123\n");
  puts(L"Copyright (C) 2020-2022 yywd_123\n");
  puts(L"[ INFO ] \x7cfb\x7edf\x521d\x59cb\x5316\x6210\x529f!!\n");

  putc('\n');
  puts(L"啊");

  puts(L"awa\n");

  int SYSStat = SYSMain();

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
        EXPECTION_HANDLER(SYSStat, 0, true);
      }
      break;
  }
}

