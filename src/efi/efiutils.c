#include <efi/efi.h>
#include <attribute.h>

static EFI_HANDLE gImageHandle __INITDATA;
static EFI_SYSTEM_TABLE *gSystemTable __INITDATA;

void __INIT
initializeEfiUtils(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
  gImageHandle = imageHandle;
  gSystemTable = systemTable;
}

void __INIT
efiPuts(const unsigned short *s) {
  eficall(gSystemTable->ConOut->OutputString, gSystemTable->ConOut, s);
}