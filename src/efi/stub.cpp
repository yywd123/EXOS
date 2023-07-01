#include <efi/efi.h>

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

void initializeKernel();

extern "C"
uint64_t __INIT
efiEntry(Handle imageHandle, EfiSystemTable *systemTable) {
  initializeEfiUtils(imageHandle, systemTable);
  efiClearScreen();

  efiPuts(L"imageHandle: 0x");
  printUInt((uint64_t)imageHandle, 16);
  efiPuts(L"\nsystemTable: 0x");
  printUInt((uint64_t)systemTable, 16);

  initializeKernel();

  while (1);

  return 0;
}
