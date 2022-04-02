/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-4-1
*/

#include <OSBase.h>

void io_hlt();

int KernelInit(unsigned long magic, unsigned long addr);

int KernelInit(unsigned long magic, unsigned long addr)
{
  if(magic != 0xe85250d6 || addr & 7) return -1;
  struct multiboot_tag *tag;
}
