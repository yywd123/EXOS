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
  
  //  Shell
  puts(L"/ |root| $>");
  return SYSStat;
}

void EXOSAPI KernelMain(void)
{
  KRNLSTAT InitStat = KernelInit();
  if(InitStat != 0)
  {
    switch (InitStat)
    {
    case INIT_FAILED_NO_MMAP:
      EXPECTION_HANDLER(INIT_FAILED_NO_MMAP, EXPECTION_INIT_FAILURE, false);
      break;
    
    default:
      printf(L"[ERROR] 内核初始化异常! 状态码: %x\n", InitStat);
      io_hlt();
      break;
    }
  }
  else printf(L"[ INFO ] Init Successed!!\n\n");

  printf(L"EXOS v0.1a 作者:yywd_123\n"
         L"Copyright (C) 2020-2022 yywd_123\n\n");

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

