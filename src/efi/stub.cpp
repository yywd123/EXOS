#include <efi/efi.h>
#include <exos/efifb.hpp>
#include <exos/serial.hpp>
#include <exos/fbcon.hpp>

USE(EXOS::Drivers);

void initializeKernel();

void __INIT
initializeInitHeapAllocator();

static inline void printUInt(uint64_t i, uint8_t n) {  //  统一打印十六进制
  if (i == 0) {
    for(int i = 0; i < n; ++i) FbConsole::print('0');
    return;
  } 
  const char *digits = "0123456789abcdef";
  char buf[17] = {0};
  for (uint8_t j = 16; j != 0; --j) {
    buf[j - 1] = digits[i & 0xf];
    i >>= 4;
  }

  const char *p = &buf[16 - n];

  FbConsole::print(p);
}

extern "C"
uint64_t __INIT
efiEntry(Handle imageHandle, EfiSystemTable *systemTable) {
  eficall(systemTable->BootServices->SetWatchdogTimer, 0, 0, 0, nullptr);
  initializeEfiUtils(imageHandle, systemTable);
  efiClearScreen();
  initializeInitHeapAllocator();

  Serial::initializeSerialPorts();

  EfiFb::initialize();
  FbConsole::initialize();

  initializeKernel();

  while (1);

  return 0;
}
