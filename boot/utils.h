#pragma once

#include <efi.h>
#include <efilib.h>
#include <bootloader.h>

#define nullptr ((void*)0)
typedef _Bool bool;
#define true 1
#define false 0

#define call(func, ...) uefi_call_wrapper(func, 0, ##__VA_ARGS__)

extern BootConfig kernelParam;

//杂项

bool compareGuid(EFI_GUID *guid1, EFI_GUID *guid2);
uint64_t getMemoryMap(uint64_t *entryCount, uint64_t *mapKey, uint64_t *descriptorSize, uint32_t *descriptorVersion);

//显卡

void switchToBestResolution(EFI_HANDLE imageHandle);

//文件系统

EFI_FILE_HANDLE openRootFs(EFI_HANDLE imageHandle);
UINTN readFile(EFI_FILE_HANDLE fs, CHAR16 *filePath, EFI_PHYSICAL_ADDRESS *address, EFI_MEMORY_TYPE memoryType, uint64_t fileOffset, uint64_t readSize);

//分页

/*
*   创建三级页表
*
*   @param 内存总页数
*
*   @retval pdpt的物理地址
*/
EFI_PHYSICAL_ADDRESS createPageTable(uint64_t pageCount);

BOOLEAN mapMemory(EFI_PHYSICAL_ADDRESS src, EFI_VIRTUAL_ADDRESS dest, UINT64 n);