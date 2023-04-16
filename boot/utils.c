#include <utils.h>

bool memcmp(void *p1, void *p2, uint64_t size) {
  if (p1 == p2) return true;
  uint64_t sz = 0;
  uint8_t *_p1 = (uint8_t*)p1;
  uint8_t *_p2 = (uint8_t*)p2;
  while (++sz < size && _p1[sz] == _p2[sz]);
  return sz == size;
}

bool compareGuid(EFI_GUID *guid1, EFI_GUID *guid2) {
  return memcmp(guid1, guid2, sizeof(EFI_GUID));
}

void switchToBestResolution(EFI_HANDLE imageHandle) {
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = nullptr;

  call(gBS->LocateProtocol, &gEfiGraphicsOutputProtocolGuid, nullptr, (void**)&gop);

  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *modeInfo;
  uint64_t infoSize;
  uint64_t pixelCount = gop->Mode->Info->PixelsPerScanLine * gop->Mode->Info->VerticalResolution;
  uint32_t modeIndex = gop->Mode->Mode;

  for (uint64_t i = 0; i < gop->Mode->MaxMode; ++i) {
    call(gop->QueryMode, gop, i, &infoSize, &modeInfo);
    if ((modeInfo->PixelsPerScanLine * modeInfo->VerticalResolution) >= pixelCount) {
      pixelCount = modeInfo->PixelsPerScanLine * modeInfo->VerticalResolution;
      modeIndex = i;
    }
  }
  call(gop->SetMode, gop, modeIndex);

  kernelParam.graphicsInfo.fbAddress = gop->Mode->FrameBufferBase;
  kernelParam.graphicsInfo.height = gop->Mode->Info->VerticalResolution;
  kernelParam.graphicsInfo.width = gop->Mode->Info->PixelsPerScanLine;

  Print(L"fb=0x%x, w=%u, h=%u\n", kernelParam.graphicsInfo.fbAddress, kernelParam.graphicsInfo.height, kernelParam.graphicsInfo.width);
}

__attribute__((const)) static EFI_DEVICE_PATH *getRootFsDevicePath(EFI_HANDLE imageHandle) {
  EFI_LOADED_IMAGE_PROTOCOL *loadedImageProtocol;
  EFI_DEVICE_PATH *rootFsDevicePath;
  call(
    gBS->OpenProtocol,
    imageHandle,
    &gEfiLoadedImageProtocolGuid,
    (void**)&loadedImageProtocol,
    imageHandle,
    nullptr,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  
  call(
    gBS->OpenProtocol,
    loadedImageProtocol->DeviceHandle,
    &gEfiDevicePathProtocolGuid,
    (void**)&rootFsDevicePath,
    imageHandle,
    nullptr,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  
  return rootFsDevicePath;
}

EFI_FILE_HANDLE openRootFs(EFI_HANDLE imageHandle) {
  EFI_FILE_IO_INTERFACE *rootVolume;
  EFI_FILE_HANDLE rootDir;
  EFI_DEVICE_PATH *rootFsDevicePath = getRootFsDevicePath(imageHandle);
  EFI_HANDLE deviceHandle;
  call(gBS->LocateDevicePath, &gEfiSimpleFileSystemProtocolGuid, &rootFsDevicePath, &deviceHandle);
  call(gBS->HandleProtocol ,deviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&rootVolume);
  call(rootVolume->OpenVolume, rootVolume, &rootDir);

  return rootDir;
}

static UINTN getFileNameLength(CHAR16 *filePath) {
  UINTN length = 0, i = 0;
  while (filePath[i] != 0) {
    if (filePath[i] == L'\\') {
      length = 0;
      ++i;
      continue;
    }
    ++length;
    ++i;
  }
  return length;
}

UINTN readFile(EFI_FILE_HANDLE fs, CHAR16 *filePath, EFI_PHYSICAL_ADDRESS *address, EFI_MEMORY_TYPE memoryType, uint64_t fileOffset, uint64_t readSize) {
  EFI_STATUS status = EFI_SUCCESS;
  
  EFI_FILE_HANDLE file;
  status = call(fs->Open, fs, &file, filePath, EFI_FILE_MODE_READ, 0);

  if (EFI_ERROR(status)) {
    Print(L"ERROR: Opening file %s with error [0x%x]\n", status);
    return status;
  }
  EFI_FILE_INFO *fileInfo;
  uint64_t infoSize = sizeof(EFI_FILE_INFO) + (getFileNameLength(filePath) + 1) * sizeof(CHAR16);
  call(gBS->AllocatePool, EfiLoaderData, infoSize, (void**)&fileInfo);
  EFI_GUID fileInfoGuid = EFI_FILE_INFO_ID;
  call(file->GetInfo, file, &fileInfoGuid, &infoSize, fileInfo);

  if (fileOffset + readSize > fileInfo->FileSize) return EFI_INVALID_PARAMETER;
  
  uint64_t pageCount = (readSize == 0 ? fileInfo->FileSize : readSize) / EFI_PAGE_SIZE + 1;

  call(
    gBS->AllocatePages,
    AllocateAddress,//*address == 0 ? AllocateAnyPages : AllocateAddress, 
    memoryType, 
    pageCount,
    address);
  if (fileOffset != 0) call(file->SetPosition, file, fileOffset);
  
  call(file->Read, file, (readSize == 0 ? &fileInfo->FileSize : &readSize), (void*)*address);

  call(file->Close, file);
  call(gBS->FreePool, fileInfo);

  return pageCount;
}

static bool growBuffer(EFI_STATUS *status, void **buffer, uint64_t bufferSize){
  bool tryAgain = false;

  if (!*buffer && bufferSize) {
    *status = EFI_BUFFER_TOO_SMALL;
  }

  if (*status == EFI_BUFFER_TOO_SMALL) {
      if (*buffer) {
        call(gBS->FreePool, *buffer);
      }

      call(gBS->AllocatePool, EfiLoaderData, bufferSize, buffer);

      if (*buffer) {
        tryAgain = true;
      } else {   
        *status = EFI_OUT_OF_RESOURCES;
      } 
  }

  if (!tryAgain && EFI_ERROR(*status) && *buffer) {
    call(gBS->FreePool, *buffer);
    *buffer = NULL;
  }

  return tryAgain;
}

typedef struct {
  UINT32  Type;
  UINT32  Pad;
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  EFI_VIRTUAL_ADDRESS  VirtualStart;
  UINT64  NumberOfPages;
  UINT64  Attribute;

  UINT64  Reserved0;
} __attribute__((packed)) MemoryBlock;

uint64_t getMemoryMap(uint64_t *entryCount, uint64_t *mapKey, uint64_t *descriptorSize, uint32_t *descriptorVersion) {
  EFI_STATUS              status;
  EFI_MEMORY_DESCRIPTOR   *buffer;
  UINTN                   bufferSize;

  status = EFI_SUCCESS;
  buffer = NULL;
  bufferSize = sizeof(EFI_MEMORY_DESCRIPTOR);

  while (growBuffer (&status, (void**) &buffer, bufferSize)) {
    status = call(gBS->GetMemoryMap, &bufferSize, buffer, mapKey, descriptorSize, descriptorVersion);
  }

  if (!EFI_ERROR(status)) {
    *entryCount = bufferSize / *descriptorSize;
  } else {
    Print(L"err %u\n", status);
    while(1);
  }

  uint64_t pageCount = 0;

  for (UINTN i = 0; i < *entryCount; ++i) {
    MemoryBlock *mb = &((MemoryBlock*)buffer)[i];
    mb->VirtualStart = mb->PhysicalStart;
    pageCount += mb->NumberOfPages;
  }

  kernelParam.efiMemoryMap = buffer;
  kernelParam.memoryMapEntryCount = *entryCount;

  return pageCount;
}
