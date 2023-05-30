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
efiClearScreen() {
  eficall(gSystemTable->ConOut->ClearScreen, gSystemTable->ConOut);
}

void __INIT
efiPuts(const unsigned short *s) {
  eficall(gSystemTable->ConOut->OutputString, gSystemTable->ConOut, s);
}

void __INIT
efiExit(uint64_t status) {
  eficall(gSystemTable->BootServices->Exit, status, 0, 0);
}

void __INIT
efiEnterKernelRuntimeStage() {  // 退出uefi的启动时服务并跳转到内核
  
}
