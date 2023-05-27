#include <efi/efi.h>
#include <efi/efilib.h>
#include <attribute.h>

uint64_t __INIT
efi_main(uintptr_t imageBase, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
  initializeEfiUtils(imageHandle, systemTable);
  eficall(systemTable->ConOut->ClearScreen, systemTable->ConOut);

  efiPuts(L"qwq\n");

  while (1);

  return 0;
}
