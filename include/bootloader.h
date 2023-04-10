#pragma once

typedef struct {
  uintptr_t fbAddress;
  uint32_t width;
  uint32_t height;
} __attribute__((packed)) GOPInfo;

typedef struct {
  GOPInfo graphicsInfo;

  void *efiRuntimeService;
  void *efiConfigurationTable;
  uint64_t tableEntryCount;

  void *efiMemoryMap;
  uint64_t memoryMapEntryCount;
} __attribute__((packed)) BootConfig;
