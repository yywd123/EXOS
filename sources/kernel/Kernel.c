/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-5-4
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
          Vinfo.BackGround_Color = 0xff000000;
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
}


void EXOSAPI KernelMain(void)
{
  KernelInit();

  uint64_t SYSStat = 1;

  InitSerialPort(COM1);
  //WriteSerialPort(COM1, 'A');
  printk(LOG_INFO, "Kernel Init Success!!");

  Vinfo.BackGround_Color = 0xff008000;
  puts(L"EXOS v0.1a \x4f5c\x8005:yywd_123\n");
  puts(L"Copyright (C) 2020-2022 yywd_123\n");
  Vinfo.BackGround_Color = 0xff000000;
  /*
  if(IsCmdLineExist) putc('1');
  else if(IsLoaderNameExist) putc('2');
  else if(IsMemInfoExist) putc('3');
  else putc('!');
*/

  puts(L"[ INFO ] \x7cfb\x7edf\x521d\x59cb\x5316\x6210\x529f\b\ba\ba\baw\bw\bwa\ba\ba!!\n");

  //for(uint16_t c = 0; c <= 0xFFFF; ++c) putc(c);
  puts(L"a b c d e f g h i j k l m n o p q r s t u v w x y z\n");
  putc('\n');
  puts(L"啊");

  if(ReadSerialPort(COM1) == 'A') putc('+');
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
