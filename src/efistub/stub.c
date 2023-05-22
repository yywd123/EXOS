#include <efi.h>
#include <efilib.h>
#include <ksegment.h>

EFIAPI
EFI_STATUS __INIT efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
  InitializeLib(imageHandle, systemTable);

  while (1);
}
