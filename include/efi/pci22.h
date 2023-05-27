#ifndef _PCI22_H
#define _PCI22_H

/*++

Copyright (c) 1999  Intel Corporation

Module Name:

    pci22.h
    
Abstract:      
    Support for PCI 2.2 standard.




Revision History

--*/

#ifdef SOFT_SDV
#define PCI_MAX_BUS     1
#else
#define PCI_MAX_BUS     255
#endif

#define PCI_MAX_DEVICE  31
#define PCI_MAX_FUNC    7

//
// Command
//
#define PCI_VGA_PALETTE_SNOOP_DISABLED   0x20

#pragma pack(1)
typedef struct {
    uint16_t      VendorId;
    uint16_t      DeviceId;
    uint16_t      Command;
    uint16_t      Status;
    uint8_t       RevisionID;
    uint8_t       ClassCode[3];
    uint8_t       CacheLineSize;
    uint8_t       LaytencyTimer;
    uint8_t       HeaderType;
    uint8_t       BIST;
} PCI_DEVICE_INDEPENDENT_REGION;

typedef struct {
    uint32_t      Bar[6];
    uint32_t      CISPtr;
    uint16_t      SubsystemVendorID;
    uint16_t      SubsystemID;
    uint32_t      ExpansionRomBar;
    uint32_t      Reserved[2];
    uint8_t       InterruptLine;
    uint8_t       InterruptPin;
    uint8_t       MinGnt;
    uint8_t       MaxLat;     
} PCI_DEVICE_HEADER_TYPE_REGION;

typedef struct {
    PCI_DEVICE_INDEPENDENT_REGION   Hdr;
    PCI_DEVICE_HEADER_TYPE_REGION   Device;
} PCI_TYPE00;

typedef struct {              
    uint32_t      Bar[2];
    uint8_t       PrimaryBus;
    uint8_t       SecondaryBus;
    uint8_t       SubordinateBus;
    uint8_t       SecondaryLatencyTimer;
    uint8_t       IoBase;
    uint8_t       IoLimit;
    uint16_t      SecondaryStatus;
    uint16_t      MemoryBase;
    uint16_t      MemoryLimit;
    uint16_t      PrefetchableMemoryBase;
    uint16_t      PrefetchableMemoryLimit;
    uint32_t      PrefetchableBaseUpper32;
    uint32_t      PrefetchableLimitUpper32;
    uint16_t      IoBaseUpper16;
    uint16_t      IoLimitUpper16;
    uint32_t      Reserved;
    uint32_t      ExpansionRomBAR;
    uint8_t       InterruptLine;
    uint8_t       InterruptPin;
    uint16_t      BridgeControl;
} PCI_BRIDGE_CONTROL_REGISTER;

#define PCI_CLASS_DISPLAY_CTRL          0x03
#define PCI_CLASS_VGA                   0x00

#define PCI_CLASS_BRIDGE                0x06
#define PCI_CLASS_ISA                   0x01
#define PCI_CLASS_ISA_POSITIVE_DECODE   0x80

#define PCI_CLASS_NETWORK               0x02 
#define PCI_CLASS_ETHERNET              0x00
        
#define HEADER_TYPE_DEVICE              0x00
#define HEADER_TYPE_PCI_TO_PCI_BRIDGE   0x01
#define HEADER_TYPE_MULTI_FUNCTION      0x80
#define HEADER_LAYOUT_CODE              0x7f

#define IS_PCI_BRIDGE(_p) ((((_p)->Hdr.HeaderType) & HEADER_LAYOUT_CODE) == HEADER_TYPE_PCI_TO_PCI_BRIDGE)        
#define IS_PCI_MULTI_FUNC(_p)   (((_p)->Hdr.HeaderType) & HEADER_TYPE_MULTI_FUNCTION)         

typedef struct {
    PCI_DEVICE_INDEPENDENT_REGION   Hdr;
    PCI_BRIDGE_CONTROL_REGISTER     Bridge;
} PCI_TYPE01;

typedef struct {
    uint8_t   Register;
    uint8_t   Function;
    uint8_t   Device;
    uint8_t   Bus;
    uint8_t   Reserved[4];
} DEFIO_PCI_ADDR;

typedef struct {
    uint32_t  Reg     : 8;
    uint32_t  Func    : 3;
    uint32_t  Dev     : 5;
    uint32_t  Bus     : 8;
    uint32_t  Reserved: 7;
    uint32_t  Enable  : 1;
} PCI_CONFIG_ACCESS_CF8;

#pragma pack()

#define EFI_ROOT_BRIDGE_LIST    'eprb'
typedef struct {
    uint64_t           Signature;

    uint16_t          BridgeNumber;
    uint16_t          PrimaryBus;
    uint16_t          SubordinateBus;

    EFI_DEVICE_PATH *DevicePath;

    LIST_ENTRY      Link;
} PCI_ROOT_BRIDGE_ENTRY;


#define PCI_EXPANSION_ROM_HEADER_SIGNATURE        0xaa55
#define EFI_PCI_EXPANSION_ROM_HEADER_EFISIGNATURE 0x0EF1
#define PCI_DATA_STRUCTURE_SIGNATURE              EFI_SIGNATURE_32('P','C','I','R')

#pragma pack(1)
typedef struct {
    uint16_t          Signature;              // 0xaa55
    uint8_t           Reserved[0x16];
    uint16_t          PcirOffset;
} PCI_EXPANSION_ROM_HEADER;


typedef struct {
    uint16_t          Signature;              // 0xaa55
    uint16_t          InitializationSize;
    uint16_t          EfiSignature;           // 0x0EF1
    uint16_t          EfiSubsystem;
    uint16_t          EfiMachineType;
    uint8_t           Reserved[0x0A];
    uint16_t          EfiImageHeaderOffset;
    uint16_t          PcirOffset;
} EFI_PCI_EXPANSION_ROM_HEADER;

typedef struct {
    uint32_t          Signature;              // "PCIR" 
    uint16_t          VendorId;
    uint16_t          DeviceId;
    uint16_t          Reserved0;
    uint16_t          Length;
    uint8_t           Revision;
    uint8_t           ClassCode[3];
    uint16_t          ImageLength;
    uint16_t          CodeRevision;
    uint8_t           CodeType;
    uint8_t           Indicator;
    uint16_t          Reserved1;
} PCI_DATA_STRUCTURE;
#pragma pack()

#endif
    




    
