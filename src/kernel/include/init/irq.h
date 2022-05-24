/*  Interrupt Request Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-17
*/

#define SYSCALL_VECTOR 0x40

uint32_t TestErrHandle(void)
{
  puts(L"qwq");
  iret;
}

uint32_t TestIRQHandle(void)
{
  puts(L"qwer");
  iret;
}

const uint32_t Interrupts_List[223] =
{
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestErrHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
  &TestIRQHandle,
};

void EXOSAPI IRQ_Init(void)
{
  for(int i = 0; i < 224; ++i)
  {
    int vector = 0x20 + i;
    if(vector != SYSCALL_VECTOR) set_intr_gate(vector, Interrupts_List[i]);
  }
}

void EXOSAPI Maskirq(uint8_t IRQ)
{
  uint16_t port;
  uint8_t data;

  if(IRQ < 8) port = PIC0_IMR;
  else
  {
    port = PIC1_IMR;
    IRQ -= 8;
  }

  data = inb(port) | (1 << IRQ);
  outb(port, data);
}

void EXOSAPI ClearMaskirq(uint8_t IRQ)
{
  uint16_t port;
  uint8_t data;

  if(IRQ < 8) port = PIC0_IMR;
  else
  {
    port = PIC1_IMR;
    IRQ -= 8;
  }

  data = inb(port) & ~(1 << IRQ);
  outb(port, data);
}
