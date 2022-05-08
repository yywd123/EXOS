#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define COM5 0x5F8
#define COM6 0x4F8
#define COM7 0x5E8
#define COM8 0x4E8

static uint8_t EXOSAPI InitSerialPort(uint16_t port)
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

static int EXOSAPI IsSerialReceived(uint16_t port) 
{
  return inb(port + 5) & 1;
}

static int8_t EXOSAPI ReadSerialPort(uint16_t port)
{
  //Wait
  while(IsSerialReceived(port) == 0);

  //Read
  return inb(port);
}

static int EXOSAPI IsSerialTransmitEmpty(uint16_t port)
{
  return inb(port + 5) & 0x20;
}

static void EXOSAPI WriteSerialPort(uint16_t port, uint8_t data)
{
  //Wait
  while(IsSerialTransmitEmpty(port) == 0);

  //Write
  outb(port, data);
}

static void EXOSAPI WriteSerialStr(uint16_t port, char *str)
{
  //Wait
  while(IsSerialTransmitEmpty(port) == 0);

  char data;
  //Write
  while((data = *str++) != 0) outb(port, data);
}
