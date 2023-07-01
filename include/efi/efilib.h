#pragma once


#ifdef __cplusplus
extern "C" {
#endif

void __INIT
initializeEfiUtils(Handle imageHandle, EfiSystemTable *systemTable);

Handle __INIT
efiGetImageHandle();

void __INIT
efiClearScreen();

void __INIT
efiPuts(const wchar_t *s);

void __INIT
efiExit(uint64_t status);

Handle __INIT
efiLocateProtocol(Guid *guid, Handle registration);

void __INIT
*efiAllocatePool(size_t size);

void __INIT
efiFreePool(void *pool);

void __INIT
*efiAllocatePages(uint64_t count);

void __INIT
efiFreePages(void *page, uint64_t count);

EFI_FILE_HANDLE
efiOpenRootFs();

uint64_t
efiReadFile(EFI_FILE_HANDLE fs, const wchar_t *filePath, uintptr_t *address, EFI_MEMORY_TYPE memoryType, uint64_t fileOffset, uint64_t readSize);

void __INIT
efiExitBootServices();

#ifdef __cplusplus
}
#endif