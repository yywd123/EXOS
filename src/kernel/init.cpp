#include <efi/efi.h>
#include <exos/efifb.hpp>
#include <exos/serial.hpp>

USE(EXOS::Drivers);

void
initializeInitHeapAllocator();  // {archdir}/abi/cxxabi.cpp

extern "C" void
initializeKernel() {
  initializeInitHeapAllocator();
  Serial::initializeSerialPorts();
  EfiFb::initializeEfiFb();

  EfiFb::drawRect(0, 0, EfiFb::getWidth(), EfiFb::getHeight(), 0x39c5bb);
}
