/*  System Init Part for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-24
*/

#include <OSBase.h>

unsigned long Bootinfo_addr;

extern BootInfo BOOTINFO;
extern VideoInfo Vinfo; 

extern KRNLSTAT EXOSAPI KernelMain(void);

// Memory Init
extern MMAP EXOSAPI MMap_Init(void);
extern void EXOSAPI Page_Init(void);

KRNLSTAT EXOSAPI KernelInit(void)
{
  KRNLSTAT InitStat = 0;

  //  Get Memory Map
  if(!BOOTINFO.MemMap) return INIT_FAILED_NO_MMAP;

  MMAP MemMap = MMap_Init();

  wprintf(L"\nFrameBuffer Address: 0x%x", Vinfo.fb_addr);
  wprintf(L"\nKernel Address: 0x%x", &KernelMain);

  //Page_Init();

  InitSerialPort(COM1);

  printk(LOG_INFO, "Kernel Init Success!!");
  wputs(L"\n[ INFO ] 系统初始化成功!!\n");
  
  //  Print SystemInfo
  wprintf(L"\nSystem Information:\n"
         L"    Memory:  Total:        %dMB\n"
         L"             Available:    %dMB\n"
         L"    CPU:  None\n"
         L"\n\n",
         MemMap.MemTotal, MemMap.MemAvailable);

  //GetCPUBrand();

  return InitStat;
}
