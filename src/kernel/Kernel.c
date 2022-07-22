/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-5-4
*/

#include <OSBase.h>

extern KRNLSTAT KernelInit(BootInfo *BInfo);
extern VideoInfo Vinfo;

KRNLSTAT EXOSAPI SYSMain(void)
{ 
  KRNLSTAT SYSStat = 0;
  //EXPECTION_HANDLER(INIT_FAILED_NO_MMAP, 0x1, true);

  //  Logo
  wprintf(L"    _______  ______  _____\n"
          L"   / ____/ |/ / __ \\/ ___/\n"
          L"  / __/  |   / / / /\\__ \\ \n"
          L" / /___ /   / /_/ /___/ / \n"
          L"/_____//_/|_\\____//____/ V0.1a 作者:yywd_123\n"
          L"Copyright (C) 2020-2022 yywd_123\n\n");
  //  Shell
  //wputs(L"/ |root| $>");
  //GraphicTest();

  DrawBlock(0, 0, Vinfo.Screen_height, Vinfo.Screen_width, 0xff000000);

  DisplayBmp((uint8_t*)0x1000000, 0, 0);
  while(1);
  return SYSStat;
}

void EXOSAPI KernelMain(BootInfo *BInfo)
{
  KRNLSTAT InitStat = KernelInit(BInfo);
  if(InitStat != 0)
  {
    switch (InitStat)
    {
    case INIT_FAILED_NO_MMAP:
      EXPECTION_HANDLER(InitStat, EXPECTION_INIT_FAILURE, false);
      break;
    
    default:
      wprintf(L"[ERROR] 内核初始化异常! 状态码: %x\n", InitStat);
      io_hlt();
      break;
    }
  }
  else wprintf(L"[ INFO ] Init Successed!!\n");

  KRNLSTAT SYSStat = SYSMain();

  wprintf(L"\n[ INFO ] Kernel Halted with Status 0x%x (%d)\n", SYSStat, SYSStat);

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

