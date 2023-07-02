#include <efi/efi.h>
#include <exos/efifb.hpp>
#include <exos/serial.hpp>
#include <exos/fbcon.hpp>

USE(EXOS::Drivers);

void
initializeInitHeapAllocator();  // {archdir}/abi/cxxabi.cpp

__NAMESPACE_DECL(Platform::SMP)
void __INIT
initBootstrapProcessor();
__NAMESPACE_END

static inline void printUInt(uint64_t i, uint8_t n) {  //  统一打印十六进制
  if (i == 0) {
    for(int i = 0; i < n; ++i) efiPuts(L"0");
    return;
  } 
  const char *digits = "0123456789abcdef";
  wchar_t buf[17] = {0};
  for (uint8_t j = 16; j != 0; --j) {
    buf[j - 1] = digits[i & 0xf];
    i >>= 4;
  }

  const wchar_t *p = &buf[16 - n];

  efiPuts(p);
}

void
initializeKernel() {
  //initializeInitHeapAllocator();
  Serial::initializeSerialPorts();
  EfiFb::initializeEfiFb();
  FbConsole::initialize();
  EfiFb::drawRect({0, 0}, EfiFb::getWidth(), EfiFb::getHeight(), 0x39c5bb);
  FbConsole::print("♂️");
  //EXOS::Platform::SMP::initBootstrapProcessor();
}
