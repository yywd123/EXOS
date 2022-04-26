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
  outb(0x00, port + 1);
  outb(0x80, port + 3);
  outb(0x03, port + 0);
  outb(0x00, port + 1);
  outb(0x03, port + 3);
  outb(0xc7, port + 2);
  outb(0x0b, port + 4);
  outb(0x1e, port + 4);
  outb(0xae, port + 0); //checksum

  // Check
  if(inb(port) != 0xae) return 0xff;

  outb(0x0f, port + 4);
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

static void EXOSAPI WriteSerialPort(uint8_t data, uint16_t port)
{
  //Wait
  while(IsSerialTransmitEmpty(port) == 0);

  //Write
  outb(data, port);
}
