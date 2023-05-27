#ifndef _LIB_SMBIOS_H
#define _LIB_SMBIOS_H
/*++

Copyright (c) 2000  Intel Corporation

Module Name:

    LibSmbios.h
    
Abstract:

    Lib include  for SMBIOS services. Used to get system serial number and GUID

Revision History

--*/

//
// Define SMBIOS tables.
//
#pragma pack(1)
typedef struct {
    uint8_t   AnchorString[4];
    uint8_t   EntryPointStructureChecksum;
    uint8_t   EntryPointLength;
    uint8_t   MajorVersion;
    uint8_t   MinorVersion;
    uint16_t  MaxStructureSize;
    uint8_t   EntryPointRevision;
    uint8_t   FormattedArea[5];
    uint8_t   IntermediateAnchorString[5];
    uint8_t   IntermediateChecksum;
    uint16_t  TableLength;
    uint32_t  TableAddress;
    uint16_t  NumberOfSmbiosStructures;
    uint8_t   SmbiosBcdRevision;
} SMBIOS_STRUCTURE_TABLE;

//
// Please note that SMBIOS structures can be odd byte aligned since the
//  unformated section of each record is a set of arbitrary size strings.
//

typedef struct {
    uint8_t   Type;
    uint8_t   Length;
    uint8_t   Handle[2];
} SMBIOS_HEADER;

typedef uint8_t   SMBIOS_STRING;

typedef struct {
    SMBIOS_HEADER   Hdr;
    SMBIOS_STRING   Vendor;
    SMBIOS_STRING   BiosVersion;
    uint8_t           BiosSegment[2];
    SMBIOS_STRING   BiosReleaseDate;
    uint8_t           BiosSize;
    uint8_t           BiosCharacteristics[8];
} SMBIOS_TYPE0;

typedef struct {
    SMBIOS_HEADER   Hdr;
    SMBIOS_STRING   Manufacturer;
    SMBIOS_STRING   ProductName;
    SMBIOS_STRING   Version;
    SMBIOS_STRING   SerialNumber;

    //
    // always byte copy this data to prevent alignment faults!
    //
    EFI_GUID        Uuid;
    
    uint8_t           WakeUpType;
} SMBIOS_TYPE1;

typedef struct {
    SMBIOS_HEADER   Hdr;
    SMBIOS_STRING   Manufacturer;
    SMBIOS_STRING   ProductName;
    SMBIOS_STRING   Version;
    SMBIOS_STRING   SerialNumber;
} SMBIOS_TYPE2;

typedef struct {
    SMBIOS_HEADER   Hdr;
    SMBIOS_STRING   Manufacturer;
    uint8_t           Type;
    SMBIOS_STRING   Version;
    SMBIOS_STRING   SerialNumber;
    SMBIOS_STRING   AssetTag;
    uint8_t           BootupState;
    uint8_t           PowerSupplyState;
    uint8_t           ThermalState;
    uint8_t           SecurityStatus;
    uint8_t           OemDefined[4];
} SMBIOS_TYPE3;

typedef struct {
    SMBIOS_HEADER   Hdr;
    uint8_t           Socket;
    uint8_t           ProcessorType;
    uint8_t           ProcessorFamily;
    SMBIOS_STRING   ProcessorManufacture;
    uint8_t           ProcessorId[8];
    SMBIOS_STRING   ProcessorVersion;
    uint8_t           Voltage;
    uint8_t           ExternalClock[2];
    uint8_t           MaxSpeed[2];
    uint8_t           CurrentSpeed[2];
    uint8_t           Status;
    uint8_t           ProcessorUpgrade;
    uint8_t           L1CacheHandle[2];
    uint8_t           L2CacheHandle[2];
    uint8_t           L3CacheHandle[2];
} SMBIOS_TYPE4;

typedef union {
    SMBIOS_HEADER   *Hdr;
    SMBIOS_TYPE0    *Type0;
    SMBIOS_TYPE1    *Type1;
    SMBIOS_TYPE2    *Type2;
    SMBIOS_TYPE3    *Type3;
    SMBIOS_TYPE4    *Type4;
    uint8_t           *Raw;
} SMBIOS_STRUCTURE_POINTER;
#pragma pack()


#endif

