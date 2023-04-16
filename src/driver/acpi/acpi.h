#pragma once

#include <types>

typedef struct {
  char signature[8];
  uint8_t checksum;
  char oemID[6];
  uint8_t revision;
  uint32_t rsdtAddress;
  uint32_t length;
  uintptr_t xsdtAddress;
  uint8_t extendedChecksum;
  uint8_t reserved[3];
} __attribute__ ((packed)) AcpiRsdp;

typedef struct {
  uint32_t signature;
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  uint8_t oem[6];
  uint8_t oemTableID[8];
  uint32_t oemVersion;
  uint32_t creatorID;
  uint32_t creatorVersion;
} __attribute__ ((packed)) AcpiHeader;

typedef struct {
  AcpiHeader header;
  uint32_t entries[0];
} __attribute__ ((packed)) AcpiRsdt;

typedef struct {
  AcpiHeader header;
  uintptr_t entries[0];
} __attribute__ ((packed)) AcpiXsdt;

typedef struct {
  uint8_t addressSpaceID;
  uint8_t registerBitWidth;
  uint8_t registerBitOffset;
  uint8_t accessWidth;    //  acpi 3.0
  uintptr_t address;
} __attribute__ ((packed)) AcpiAddress;

typedef struct {
  AcpiHeader header;
  uint32_t firmwareControl;
  uint32_t dsdt;
  uint8_t reserved0;
  uint8_t preferredPMProfile;
  uint16_t sciInterrupt;
  uint32_t smiCommandPort;
  uint8_t acpiEnable;
  uint8_t acpiDisable;
  uint8_t s4BiosRequestValue;
  uint8_t pStateControlValue;
  uint32_t pm1aEventBlock;
  uint32_t pm1bEventBlock;
  uint32_t pm1aControlBlock;
  uint32_t pm1bControlBlock;
  uint32_t pm2ControlBlock;
  uint32_t powerManagementTimer;
  uint32_t gpEvent0Block;
  uint32_t gpEvent1Block;
  uint8_t pm1EventBlockLength;
  uint8_t pm1ControlBlockLength;
  uint8_t pm2ControlBlockLength;
  uint8_t pmTimerLength;
  uint8_t gpEvent0Length;
  uint8_t gpEvent1Length;
  uint8_t gpEvent1Offset;
  uint8_t cStateControlValue;
  uint16_t level2HardwareTimeout;
  uint16_t level3HardwareTimeout;
  uint16_t flushSize;
  uint16_t flushStride;
  uint8_t dutyOffset;
  uint8_t dutyWidth;
  uint8_t dayAlarm;
  uint8_t monthAlarm;
  uint8_t century;
  uint16_t iaPcBootArch;
  uint8_t reserved1;
  uint32_t flags;
  AcpiAddress resetRegister;
  uint8_t resetValue;
  uint16_t armBootArch;
  uint8_t fadtMinorVersion;
  uintptr_t xFirmwareControl;
  uintptr_t xDsdt;
  AcpiAddress xPM1aEventBlock;
  AcpiAddress xPM1bEventBlock;
  AcpiAddress xPM1aControlBlock;
  AcpiAddress xPM1bControlBlock;
  AcpiAddress xPM2ControlBlock;
  AcpiAddress xPowerManagementTimerBlock;
  AcpiAddress xGPEvent0Block;
  AcpiAddress xGPEvent1Block;
  AcpiAddress sleepControlRegister;
  AcpiAddress sleepStatusRegister;
  uint64_t hypevisorID;
} __attribute__ ((packed)) AcpiFadt;

typedef struct {
  AcpiHeader header;
  uint8_t data[0];
} __attribute__ ((packed)) AcpiDsdt;

typedef struct {
  AcpiHeader header;
  uint8_t status;
  uint8_t imageType;
  void *imagePtr;
  uint32_t xOffset;
  uint32_t yOffset;
} __attribute__ ((packed)) AcpiBgrt;

typedef AcpiDsdt AcpiSsdt;

typedef struct {
  AcpiHeader header;
  uint32_t lapicAddr;
  uint32_t flags;
  uint8_t data[0];
} __attribute__ ((packed)) AcpiMadt;

void acpiInit(void *rsdptr);