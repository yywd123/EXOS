/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-5-4
*/

#include <OSBase.h>

BootInfo BOOTINFO;
VideoInfo Vinfo;

extern KRNLSTAT KernelInit(void);

KRNLSTAT EXOSAPI SYSMain(void)
{ 
  KRNLSTAT SYSStat = 0;
  //EXPECTION_HANDLER(SYSStat, 0, true);
  puts(L"Welcome to EXOS V0.0.1\n");
  puts(L"\n \\\u30a2\u30c3\u30ab\u30ea\uff5e\u30f3/\n\n");      //don't delete it (doge
  puts(L"/ |root| $>");
  return SYSStat;
}

void EXOSAPI KernelMain(void)
{
  KRNLSTAT InitStat = KernelInit();
  printf(L"\n\n[ INFO ] Init Successed with Status 0x%x (%d)\n\n", InitStat, InitStat);

  printf(L"EXOS v0.1a \x4f5c\x8005:yywd_123\n"
         L"Copyright (C) 2020-2022 yywd_123\n\n");

  printf(L"MemoryInfo:\n"
         L"  MEM_upper: %d\n"
         L"  MEM_lower: %d\n\n",
         BOOTINFO.mem_upper, BOOTINFO.mem_lower);

  KRNLSTAT SYSStat = SYSMain();

  printf(L"\n\n[ INFO ] Kernel Halted with Status 0x%x (%d)\n", SYSStat, SYSStat);

  switch(SYSStat)
  {
    case 0:
      //Shutdown();
      io_hlt();
      break;
    case 1:
      //Reboot();
      io_hlt();
      break;
    default:
      {
        if(SYSStat < 0) EXPECTION_HANDLER(SYSStat, 0, true);
      }
      break;
  }
}

