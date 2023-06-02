#include <efi/efi.h>
#include <attribute.h>

static Handle gImageHandle __INITDATA;
static EfiSystemTable *gSystemTable __INITDATA;

extern "C"
void __INIT
initializeEfiUtils(Handle imageHandle, EfiSystemTable *systemTable) {
  gImageHandle = imageHandle;
  gSystemTable = systemTable;
}

extern "C"
Handle __INIT
efiGetImageHandle() {
  return gImageHandle;
}

extern "C"
void __INIT
efiClearScreen() {
  eficall(gSystemTable->ConOut->ClearScreen, gSystemTable->ConOut);
}

extern "C"
void __INIT
efiPuts(const wchar_t *s) {
  eficall(gSystemTable->ConOut->OutputString, gSystemTable->ConOut, s);
}

extern "C"
void __INIT
efiExit(uint64_t status) {
  eficall(gSystemTable->BootServices->Exit, status, 0, 0);
}

extern "C"
Handle __INIT
efiLocateProtocol(Guid *guid, Handle registration) {
  Handle protocolInterface = nullptr;
  eficall(gSystemTable->BootServices->LocateProtocol, guid, registration, &protocolInterface);
  return protocolInterface;
}

extern "C"
void __INIT
*efiAllocatePool(size_t size) {
  void *pool = nullptr;
  eficall(gSystemTable->BootServices->AllocatePool, EfiRuntimeServicesData, size, &pool);
  return pool;
}

extern "C"
void __INIT
efiFreePool(void *pool) {
  eficall(gSystemTable->BootServices->FreePool, pool);
}

extern "C"
void __INIT
efiEnterKernelRuntimeStage() {  // 退出uefi的启动时服务并跳转到内核
  
}
