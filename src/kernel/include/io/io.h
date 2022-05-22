/*  Base I/O Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-15
*/

#define iret __asm__ __volatile__("iret")

void EXOSAPI outb(uint16_t port, uint8_t data)
{
  __asm__ __volatile__( "outb %0, %%dx \n\t"
                        "mfence"
                        ::"a"(data), "d"(port)
                        :"memory");
}

void EXOSAPI outw(uint16_t port, uint16_t data)
{
  __asm__ __volatile__( "outw %0, %%dx \n\t"
                        "mfence"
                        ::"a"(data), "d"(port)
                        :"memory");
}

void EXOSAPI outd(uint16_t port, uint32_t data)
{
  __asm__ __volatile__( "outl %0, %%dx \n\t"
                        "mfence"
                        ::"a"(data), "d"(port)
                        :"memory");
}

uint8_t EXOSAPI inb(uint16_t port)
{
  uint8_t result = 0;
  __asm__ __volatile__( "inb %%dx, %0 \n\t"
                        "mfence"
                        :"=a"(result)
                        :"d"(port)
                        :"memory");
  return result;
}

uint16_t EXOSAPI inw(uint16_t port)
{
  uint16_t result = 0;
  __asm__ __volatile__( "inw %%dx, %0 \n\t"
                        "mfence"
                        :"=a"(result)
                        :"d"(port)
                        :"memory");
  return result;
}

uint32_t EXOSAPI ind(uint16_t port)
{
  uint32_t result = 0;
  __asm__ __volatile__( "inl %%dx, %0 \n\t"
                        "mfence"
                        :"=a"(result)
                        :"d"(port)
                        :"memory");
  return result;
}

void EXOSAPI io_hlt(void)
{
  __asm__ __volatile__("hlt");
}

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define COM5 0x5F8
#define COM6 0x4F8
#define COM7 0x5E8
#define COM8 0x4E8

uint8_t EXOSAPI InitSerialPort(uint16_t port);
int EXOSAPI IsSerialReceived(uint16_t port);
uint8_t EXOSAPI ReadSerialPort(uint16_t port);
int EXOSAPI IsSerialTransmitEmpty(uint16_t port);
void EXOSAPI WriteSerialPort(uint16_t port, uint8_t data);
void EXOSAPI WriteSerialStr(uint16_t port, char *str);

#include "serial.h"


