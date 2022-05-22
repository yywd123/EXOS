/*  Serial Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-17
*/

uint8_t EXOSAPI InitSerialPort(uint16_t port)
{
  //Init
  outb(port + 1, 0x00);
  outb(port + 3, 0x80);
  outb(port + 0, 0x03);
  outb(port + 1, 0x00);
  outb(port + 3, 0x03);
  outb(port + 2, 0xc7);
  outb(port + 4, 0x0b);
  outb(port + 4, 0x1e);
  outb(port + 0, 0xae); //checksum

  // Check
  if(inb(port) != 0xae) return 0xff;

  outb(port + 4, 0x0f);
  return 0;
}

int EXOSAPI IsSerialReceived(uint16_t port) 
{
  return inb(port + 5) & 1;
}

uint8_t EXOSAPI ReadSerialPort(uint16_t port)
{
  //Wait
  while(IsSerialReceived(port) == 0);

  //Read
  return inb(port);
}

int EXOSAPI IsSerialTransmitEmpty(uint16_t port)
{
  return inb(port + 5) & 0x20;
}

void EXOSAPI WriteSerialPort(uint16_t port, uint8_t data)
{
  //Wait
  while(IsSerialTransmitEmpty(port) == 0);

  //Write
  outb(port, data);
}

void EXOSAPI WriteSerialStr(uint16_t port, char *str)
{
  //Wait
  while(IsSerialTransmitEmpty(port) == 0);

  char data;
  //Write
  while((data = *str++) != 0) outb(port, data);
}
