#pragma once

#include <mm/paging.hpp>

__NAMESPACE_DECL(Memory::KMemory)

typedef enum {
	ReservedMemoryType,
	LoaderCode,
	LoaderData,
	BootServicesCode,
	BootServicesData,
	RuntimeServicesCode,
	RuntimeServicesData,
	ConventionalMemory,
	UnusableMemory,
	ACPIReclaimMemory,
	ACPIMemoryNVS,
	MemoryMappedIO,
	MemoryMappedIOPortSpace,
	PalCode,
	PersistentMemory,
	EfiMaxMemoryType
} MemoryType;

typedef struct {
	uint32_t type;
	uint32_t pad;
	uintptr_t physicalStart;
	uintptr_t virtualStart;
	uint32_t pageCount;	 //  4kb页
	uint32_t freePageCount;
	uint64_t attribute;

	uint32_t allocationBase;
	uint32_t reserved;
} __packed PageBlock;

void
initialize();

void *
alloc(size_t size);

void *
alloc4KPages(uint32_t count);

void *
alloc4KPages(uintptr_t address, uint32_t count);

void *
alloc2MPages(uint32_t count);

/**
 * @brief 将指定内存页释放
 * @param p 内存页指针(自动4k对齐)
 * @param count 内存页数量(4kb)
 */
void
freePages(void *p, uint32_t count);

void
free(void *p);

__NAMESPACE_END