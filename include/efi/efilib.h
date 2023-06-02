#pragma once

#include <attribute.h>
#include <basedefs.h>

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

#ifdef __cplusplus
}
#endif