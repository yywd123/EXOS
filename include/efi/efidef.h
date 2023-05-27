#ifndef _EFI_DEF_H
#define _EFI_DEF_H

#include <stdint.h>

/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efidef.h

Abstract:

    EFI definitions




Revision History

--*/

#ifndef true
#include <stdbool.h>
#endif

#ifndef NULL
    #define NULL    ((void *) 0)
#endif

typedef uint64_t           EFI_STATUS;
typedef uint64_t          EFI_LBA;
typedef uint64_t           EFI_TPL;
typedef void            *EFI_HANDLE;
typedef void            *EFI_EVENT;


//
// Prototype argument decoration for EFI parameters to indicate
// their direction
//
// IN - argument is passed into the function
// OUT - argument (pointer) is returned from the function
// OPTIONAL - argument is optional
//

#ifndef IN
    #define IN
    #define OUT
    #define OPTIONAL
#endif


//
// A GUID
//

typedef struct {          
    uint32_t  Data1;
    uint16_t  Data2;
    uint16_t  Data3;
    uint8_t   Data4[8]; 
} EFI_GUID;


//
// Time
//

typedef struct {          
    uint16_t      Year;       // 1998 - 20XX
    uint8_t       Month;      // 1 - 12
    uint8_t       Day;        // 1 - 31
    uint8_t       Hour;       // 0 - 23
    uint8_t       Minute;     // 0 - 59
    uint8_t       Second;     // 0 - 59
    uint8_t       Pad1;
    uint32_t      Nanosecond; // 0 - 999,999,999
    int16_t       TimeZone;   // -1440 to 1440 or 2047
    uint8_t       Daylight;
    uint8_t       Pad2;
} EFI_TIME;

// Bit definitions for EFI_TIME.Daylight
#define EFI_TIME_ADJUST_DAYLIGHT    0x01
#define EFI_TIME_IN_DAYLIGHT        0x02

// Value definition for EFI_TIME.TimeZone
#define EFI_UNSPECIFIED_TIMEZONE    0x07FF



//
// Networking
//

typedef struct {
    uint8_t                   Addr[4];
} EFI_IPv4_ADDRESS;

typedef struct {
    uint8_t                   Addr[16];
} EFI_IPv6_ADDRESS;

typedef struct {
    uint8_t                   Addr[32];
} EFI_MAC_ADDRESS;

typedef struct {
    uint32_t ReceivedQueueTimeoutValue;
    uint32_t TransmitQueueTimeoutValue;
    uint16_t ProtocolTypeFilter;
    bool EnableUnicastReceive;
    bool EnableMulticastReceive;
    bool EnableBroadcastReceive;
    bool EnablePromiscuousReceive;
    bool FlushQueuesOnReset;
    bool EnableReceiveTimestamps;
    bool DisableBackgroundPolling;
} EFI_MANAGED_NETWORK_CONFIG_DATA;

//
// Memory
//

typedef uint64_t          EFI_PHYSICAL_ADDRESS;
typedef uint64_t          EFI_VIRTUAL_ADDRESS;

typedef enum {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} EFI_ALLOCATE_TYPE;

//Preseve the attr on any range supplied.
//ConventialMemory must have WB,SR,SW when supplied.
//When allocating from ConventialMemory always make it WB,SR,SW
//When returning to ConventialMemory always make it WB,SR,SW
//When getting the memory map, or on RT for runtime types


typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;

// possible caching types for the memory range
#define EFI_MEMORY_UC           0x0000000000000001
#define EFI_MEMORY_WC           0x0000000000000002
#define EFI_MEMORY_WT           0x0000000000000004
#define EFI_MEMORY_WB           0x0000000000000008
#define EFI_MEMORY_UCE          0x0000000000000010  

// physical memory protection on range 
#define EFI_MEMORY_WP           0x0000000000001000
#define EFI_MEMORY_RP           0x0000000000002000
#define EFI_MEMORY_XP           0x0000000000004000

// range requires a runtime mapping
#define EFI_MEMORY_RUNTIME      0x8000000000000000

#define EFI_MEMORY_DESCRIPTOR_VERSION  1
typedef struct {
    uint32_t                          Type;           // Field size is 32 bits followed by 32 bit pad
    uint32_t                          Pad;
    EFI_PHYSICAL_ADDRESS            PhysicalStart;  // Field size is 64 bits
    EFI_VIRTUAL_ADDRESS             VirtualStart;   // Field size is 64 bits
    uint64_t                          NumberOfPages;  // Field size is 64 bits
    uint64_t                          Attribute;      // Field size is 64 bits
} EFI_MEMORY_DESCRIPTOR;

//
// International Language
//

typedef uint8_t   ISO_639_2;
#define ISO_639_2_ENTRY_SIZE    3

//
//
//

#define EFI_PAGE_SIZE   4096
#define EFI_PAGE_MASK   0xFFF
#define EFI_PAGE_SHIFT  12

#define EFI_SIZE_TO_PAGES(a)  \
    ( ((a) >> EFI_PAGE_SHIFT) + ((a) & EFI_PAGE_MASK ? 1 : 0) )

#define EFI_OS_INDICATIONS_BOOT_TO_FW_UI        0x0000000000000001
#define EFI_OS_INDICATIONS_TIMESTAMP_REVOCATION 0x0000000000000002
#define EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED \
                                                0x0000000000000004
#define EFI_OS_INDICATIONS_FMP_CAPSULE_SUPPORTED \
                                                0x0000000000000008
#define EFI_OS_INDICATIONS_CAPSULE_RESULT_VAR_SUPPORTED \
                                                0x0000000000000010

#endif
