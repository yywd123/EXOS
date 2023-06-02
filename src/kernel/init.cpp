#include <efi/efi.h>
#include <drivers/efifb.hpp>

using namespace EXOS;

extern "C"
void initializeKernel() {
  Display::DisplayImpl *display = Drivers::EfiFb::getDisplayImpl();
  display->drawRect(0, 0, 2048, 2048, 0x39c5bb);
}