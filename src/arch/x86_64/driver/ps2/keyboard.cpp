#include <arch/arch.h>
#include <input/keyboard.h>
#include <lib/Logger.hpp>

using namespace EXOS::Utils;
using namespace EXOS::Driver;

void waitForKeyboardController() {
  while (IO::inb(0x64) & 0b10);
}

void keyboardInit() {
  waitForKeyboardController();
  IO::outb(0x64, 0x60);
  waitForKeyboardController();
  IO::outb(0x60, 0x47);
  maskIRQ(1, true);
}


void handleKeyboardInput() {
  sendEOI(1);
  updateKeyboardState(PS2, IO::inb(0x60));
}