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

int32_t EXOSAPI SYSMain(void)
{ 
  int32_t SYSStat = 0x114514;
  //EXPECTION_HANDLER(SYSStat, 0, true);
  puts(L"Welcome to EXOS V0.0.1\n");
  puts(L"\n \\\u30a2\u30c3\u30ab\u30ea\uff5e\u30f3/\n\n");      //don't delete it (doge
  puts(L"/ |root| $>");
  return SYSStat;
}

void EXOSAPI KernelMain(void)
{
  KernelInit();

  puts(L"EXOS v0.1a \x4f5c\x8005:yywd_123\n");
  puts(L"Copyright (C) 2020-2022 yywd_123\n");
  putc('\n');

  wchar_t MemUpper[11] = {0}, MemLower[11] = {0};
  itol(BOOTINFO.mem_upper, MemUpper, L'd');
  itol(BOOTINFO.mem_lower, MemLower, L'd');
  puts(L"MemoryInfo:\n");
  puts(L"MEM_upper:");
  puts(MemUpper);
  puts(L"\nMEM_lower:");
  puts(MemLower);
  puts(L"\n");

  int32_t SYSStat = SYSMain();

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

