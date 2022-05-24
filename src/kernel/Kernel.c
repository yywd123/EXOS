/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-5-4
*/

#include <OSBase.h>

BootInfo BOOTINFO;
VideoInfo Vinfo;

extern void KernelInit(void);

int64_t EXOSAPI SYSMain(void)
{ 
  uint64_t SYSStat = 0;
  puts(L"Welcome to EXOS 0.0.1(build date:2022-5-21)\n");
  puts(L"root:/ $>");
  return SYSStat;
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

  int64_t SYSStat = SYSMain();

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
        EXPECTION_HANDLER(SYSStat, 0, true);
      }
      break;
  }
}

