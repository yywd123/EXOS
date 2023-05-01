#include <driver/Serial.hpp>
#include <arch/arch.h>

using namespace EXOS::Driver;

uint8_t portStatus[8];
#define FLAG_INITIALIZED BIT(0)

uint16_t getPort(Serial::SerialPort p) {
  uint16_t port = 0;
  switch (p) {
    case 0:
      port = 0x3f8;
      break;
    case 1:
      port = 0x2f8;
      break;
    case 2:
      port = 0x3e8;
      break;
    case 3:
      port = 0x2e8;
      break;
    case 4:
      port = 0x5f8;
      break;
    case 5:
      port = 0x4f8;
      break;
    case 6:
      port = 0x5e8;
      break;
    case 7:
      port = 0x4e8;
      break;
    default: return 0;
  }
  if (checkFlag(portStatus[p], FLAG_INITIALIZED)) goto initialized;

  IO::outb(port + 1, 0);
  IO::outb(port + 3, 0b10000000);
  IO::outb(port + 0, 0x01); // 0x  01 = 115200bps
  IO::outb(port + 1, 0x00); //   00
  IO::outb(port + 3, 0b00000011);
  IO::outb(port + 2, 0b11000111);
  IO::outb(port + 4, 0b00001111);
  IO::outb(port + 4, 0b00011110);

  IO::outb(port, 0x58); //  校验码
  if(IO::inb(port) != 0x58) return 0;

  IO::outb(port + 4, 0b00001111);
  portStatus[p] |= FLAG_INITIALIZED;

initialized:
  return port;
}

namespace EXOS::Driver::Serial {
  void write(SerialPort port, uint8_t byte) {
    uint16_t p = getPort(port);
    if (!p) return;
    //while (!(IO::inb(p + 5) & BIT(0)));
    IO::outb(p, byte);
  }

  uint8_t read(SerialPort port) {
    uint16_t p = getPort(port);
    if (!p) return 0;
    while (!(IO::inb(p + 5) & BIT(5)));
    return IO::inb(p);
  }
}