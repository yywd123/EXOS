#pragma once

void __INIT
initialize(Handle imageHandle, SystemTable *systemTable);

Handle __INIT
getImageHandle();

SystemTable *__INIT
getSystemTable();

void __INIT
clearScreen();

void __INIT
puts(const wchar_t *s);

void __INIT
exit(uint64_t status);

Handle __INIT
locateProtocol(UUID *guid, Handle registration);

void *__INIT
allocatePool(size_t size);

void __INIT
freePool(void *pool);

void *__INIT
allocatePages(uint64_t count);

void __INIT
freePages(void *page, uint64_t count);

EFI_FILE_HANDLE
openRootFs();

uint64_t
readFile(EFI_FILE_HANDLE fs, const wchar_t *filePath, uintptr_t *address, MemoryType memoryType, uint64_t fileOffset, uint64_t readSize);

void __INIT
exitBootServices();

#define gST EFI::getSystemTable()
#define gBS gST->bootServices
#define gRT gST->runtimeServices