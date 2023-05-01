#include <arch/arch.h>
#include <input/keyboard.h>

using namespace EXOS::Driver;

void maskIRQ(uint8_t irq, bool enable) {
  uint16_t port = PIC1_DATA;

  if (irq >= 8) {
    port = PIC2_DATA;
    irq -= 8;
  }

  uint8_t mask = IO::inb(port);
  IO::outb(port, enable ? mask & ~(1 << irq) : mask | (1 << irq));
}

void sendEOI(uint8_t irq) {
  if (irq >= 8) IO::outb(0xa0,0x20);
  IO::outb(0x20,0x20);
}

extern "C" void picInit() {
  IO::outb(PIC1_DATA, 0xff);
  IO::outb(PIC2_DATA, 0xff);

  IO::outb(PIC1_CMD, 0b00010001);
  IO::outb(PIC1_DATA, 0x20);
  IO::outb(PIC1_DATA, 0b00000100);
  IO::outb(PIC1_DATA, 0b00000001);

  IO::outb(PIC2_CMD, 0b00100000);
  IO::outb(PIC2_DATA, 0x28);
  IO::outb(PIC2_DATA, 0b00000010);
  IO::outb(PIC2_DATA, 0b00000001);

  IO::outb(PIC1_DATA, 0xff);
  IO::outb(PIC2_DATA, 0xff);

  keyboardInit();
}