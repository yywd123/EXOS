/*  Keyboard Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-15
*/

#include <OSBase.h>

int KB_INTHandler(void)
{
  puts(L"keyboard"); 
  iret;
}

void EXOSAPI WaitKeyboardReady(void)
{
  while((inb(PORT_KBStat) & 0x02) != 0);
}

void EXOSAPI KeyboardInit(void)
{
  WaitKeyboardReady();
  outb(PORT_KBCmd, 0x60);
  WaitKeyboardReady();
  outb(PORT_KBData, 0x47);
  return;
}

uint8_t EXOSAPI getc(void)
{
  uint8_t c;
  
}

uint8_t EXOSAPI KB_GetKey(void)
{
  uint8_t c;
  while(1)
  {
    io_cli();
    
  }
}
