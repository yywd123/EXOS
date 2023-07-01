#pragma once

#include <stdint.h>

#ifndef true
#include <stdbool.h>
#endif

#ifndef __cplusplus
	#define nullptr    ((void*) 0)
#endif

typedef uint64_t          EfiLba;
typedef uint64_t           EfiTpl;
typedef void            *EfiEvent;

//
// Prototype argument decoration for EFI parameters to indicate
// their direction
//
// - argument is passed into the function
// - argument (pointer) is returned from the function
// OPTIONAL - argument is optional
//

#define OPTIONAL

#include <utils/guid.h>

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
} EfiTime;

// Bit definitions for EfiTime.Daylight
#define EFI_TIME_ADJUST_DAYLIGHT    0x01
#define EFI_TIME_IN_DAYLIGHT        0x02

// Value definition for EfiTime.TimeZone
#define EFI_UNSPECIFIED_TIMEZONE    0x07FF

//
// Memory
//

typedef uint64_t          uintptr_t;
typedef uint64_t          uintptr_t;

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
	uint32_t type;
	uint32_t pad;
	uintptr_t physicalStart;
	uintptr_t virtualStart; 
	uint64_t pageCount;
	uint64_t attribute;

	uint64_t reserved;
} __packed EfiMemoryDescriptor;

#define EFI_PAGE_SIZE   4096
#define EFI_PAGE_MASK   0xFFF
#define EFI_PAGE_SHIFT  12

#define EFI_SIZE_TO_PAGES(a)  \
	( ((a) >> EFI_PAGE_SHIFT) + ((a) & EFI_PAGE_MASK ? 1 : 0) )
