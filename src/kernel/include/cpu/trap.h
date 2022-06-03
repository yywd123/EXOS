/*  Interrupts Trap Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-20
*/

void EXOSAPI set_intr_gate(GATEDESC idt, uint32_t offset, void *func)
{
  long addr = idt + offset;
  uint32_t tmp0, tmp1;
  __asm__ __volatile__( "movw %%dx, %%ax    \n\t"
                        "movw %4, %%dx      \n\t"
                        "movl %%eax, %0     \n\t"
                        "movl %%edx, %1"
                        :"=m"(*((long *) addr), "=m" (*(1 + (long *) addr)), "=&a" (tmp0), "=&d" (tmp1)
                        :"i" ((short) (0x8000 + (0 << 13) + (14 << 8))), "3" ((char *) func), "2" ((12 * 8) << 16));
}

void EXOSAPI set_trap_gate(GATEDESC idt, uint32_t offset, void *func)
{
  long addr = idt + offset;
  uint32_t tmp0, tmp1;
  __asm__ __volatile__( "movw %%dx, %%ax    \n\t"
                        "movw %4, %%dx      \n\t"
                        "movl %%eax, %0     \n\t"
                        "movl %%edx, %1"
                        :"=m"(*((long *) addr), "=m" (*(1 + (long *) addr)), "=&a" (tmp0), "=&d" (tmp1)
                        :"i" ((short) (0x8000 + (0 << 13) + (15 << 8))), "3" ((char *) func), "2" ((12 * 8) << 16));
}

void EXOSAPI set_system_gate(GATEDESC idt, uint32_t offset, void *func)
{
  long addr = idt + offset;
  uint32_t tmp0, tmp1;
  __asm__ __volatile__( "movw %%dx, %%ax    \n\t"
                        "movw %4, %%dx      \n\t"
                        "movl %%eax, %0     \n\t"
                        "movl %%edx, %1"
                        :"=m"(*((long *) addr), "=m" (*(1 + (long *) addr)), "=&a" (tmp0), "=&d" (tmp1)
                        :"i" ((short) (0x8000 + (3 << 13) + (15 << 8))), "3" ((char *) func), "2" ((12 * 8) << 16));
}

void EXOSAPI set_call_gate(GATEDESC idt, uint32_t offset, void *func)
{
  long addr = idt + offset;
  uint32_t tmp0, tmp1;
  __asm__ __volatile__( "movw %%dx, %%ax    \n\t"
                        "movw %4, %%dx      \n\t"
                        "movl %%eax, %0     \n\t"
                        "movl %%edx, %1"
                        :"=m"(*((long *) addr), "=m" (*(1 + (long *) addr)), "=&a" (tmp0), "=&d" (tmp1)
                        :"i" ((short) (0x8000 + (3 << 13) + (12 << 8))), "3" ((char *) func), "2" ((12 * 8) << 16));
}

