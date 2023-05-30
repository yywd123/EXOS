#pragma once

#include <efi/efi.h>

void initializeEfiUtils(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable);
void efiClearScreen();
void efiPuts(const unsigned short *s);
void efiExit(uint64_t status);