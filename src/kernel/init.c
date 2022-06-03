/*  System Init Part for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-24
*/

#include <OSBase.h>

unsigned long Bootinfo_addr;

extern BootInfo BOOTINFO;
extern VideoInfo Vinfo; 

MMAP EXOSAPI MMap_Init(void)
{
  MMAP MemMap;
  multiboot_memory_map_t *mmap;
  uint16_t MemBlockCount = 0, AvailableMemBlockCount = 0;

  MemMap.MemAvailable = 0;
  MemMap.MemTotal = 0;
  
  printf(L"Memory Map:\n");
  for(mmap = BOOTINFO.MemMap->entries; 
      (uint8_t *)mmap < (uint8_t *)BOOTINFO.MemMap + BOOTINFO.MemMap->size;
       mmap = (multiboot_memory_map_t *)((unsigned long) mmap + sizeof(multiboot_memory_map_t)))
  {
    if(mmap->type) 
    {
      MemMap.MemAvailable += mmap->len / (1024 * 1024);     //1024byte * 1024 = 1MiB
      MemMap.block[AvailableMemBlockCount].Base_Addr = BOOTINFO.MemMap->entries->addr;
      MemMap.block[AvailableMemBlockCount].BlockSize = BOOTINFO.MemMap->entries->len;
      ++AvailableMemBlockCount;
    }

    MemMap.MemTotal += mmap->len / 1024;
    printf(L"  Block %d, Base_Addr: 0x%x%x, Size: 0x%x%x, Type: 0x%x\n", 
        MemBlockCount,
        (unsigned)(mmap->addr >> 32), (unsigned)(mmap->addr & 0xffffffff),
        (unsigned)(mmap->len >> 32), (unsigned)(mmap->len & 0xffffffff),
        (unsigned)(mmap->type));
    ++MemBlockCount;
  }

  MemMap.MemTotal /= 1024;

  return MemMap;
}

KRNLSTAT EXOSAPI KernelInit(void)
{
  KRNLSTAT InitStat = 0;
  bool IsCmdLineExist = false;
  bool IsLoaderNameExist = false;
  bool IsModuleExist = false;
  bool IsMemInfoExist = false;
  bool IsBootDevExist = false;
  bool IsMemMapExist = false;

  //  Get BootInfo
  if(Bootinfo_addr & 7) io_hlt();   //  Check Bootinfo_addr

  struct multiboot_tag *tag;
  for(tag = (struct multiboot_tag *)(Bootinfo_addr + 8);
      tag->type != MULTIBOOT_TAG_TYPE_END;
      tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
  {
    switch(tag->type)
    {
      case MULTIBOOT_TAG_TYPE_CMDLINE:
        {
          IsCmdLineExist = true;
        }break;
      case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        {
          IsLoaderNameExist = true;
        }break;
      case MULTIBOOT_TAG_TYPE_MODULE:
        {
          BOOTINFO.ModInfo = (struct multiboot_tag_module *)tag;
          IsModuleExist = true;
        }break;
      /*case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
          BOOTINFO.mem_upper = ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper;
          BOOTINFO.mem_lower = ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower;
          IsMemInfoExist = true;
        }break;*/       //unused
      case MULTIBOOT_TAG_TYPE_BOOTDEV:
        {
          IsBootDevExist = true;
        }break;
      case MULTIBOOT_TAG_TYPE_MMAP:
        {
          BOOTINFO.MemMap = (struct multiboot_tag_mmap *)tag;
          IsMemMapExist = true;
        }break;
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
        }break;
    }
  }

  //  Get Memory Map
  if(!IsMemMapExist) return INIT_FAILED_NO_MMAP;

  MMAP MemMap = MMap_Init();

  InitSerialPort(COM1);

  printk(LOG_INFO, "Kernel Init Success!!");
  puts(L"\n[ INFO ] 系统初始化成功!!\n");
  
  //  Print SystemInfo
  printf(L"\nSystem Information:\n"
         L"    Memory:  Total:        %dMB\n"
         L"             Available:    %dMB\n"
         L"    CPU:  None\n"
         L"\n\n",
         MemMap.MemTotal, MemMap.MemAvailable);

  //GetCPUBrand();

  return InitStat;
}
