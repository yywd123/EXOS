#include <efi/efi.h>
#include <efi/efilib.h>
#include <attribute.h>

static inline void printUInt(uint64_t i, uint8_t n) {  //  统一打印十六进制
  if (i == 0) {
    for(int i = 0; i < n; ++i) efiPuts(L"0");
    return;
  } 
  const char *digits = "0123456789abcdef";
  unsigned short buf[17] = {0};
  for (uint8_t j = 16; j != 0; --j) {
    buf[j - 1] = digits[i & 0xf];
    i >>= 4;
  }

  const unsigned short *p = &buf[16 - n];

  efiPuts(p);
}


uint64_t __INIT
efi_main(uintptr_t imageBase, uintptr_t einit, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
  initializeEfiUtils(imageHandle, systemTable);
  efiClearScreen();

  efiPuts(L"imageBase: 0x");
  printUInt(imageBase, 16);
  efiPuts(L"\neinit: 0x");
  printUInt(einit, 16);

  while (1);

  return 0;
}
