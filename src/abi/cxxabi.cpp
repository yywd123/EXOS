#include <efi/efi.hpp>
#include <mm/mm.hpp>
#include <exos/logger.hpp>

USE(EXOS::Memory);
USE(EXOS::Utils);

static size_t allocatedSize = 0;
static uint16_t allocateCount = 0;

void *
operator new(size_t size) {
	Logger::log(Logger::INFO, "memory alloc: @bytes", (int64_t)size);
	++allocateCount;
	allocatedSize += size;
	return EFI::allocatePool(size);
}

void *
operator new[](size_t size) {
	Logger::log(Logger::INFO, "memory alloc: @bytes", (int64_t)size);
	++allocateCount;
	allocatedSize += size;
	return EFI::allocatePool(size);
}

void
operator delete(void *p) {
	EFI::freePool(p);
}

void
operator delete(void *p, size_t) {
	EFI::freePool(p);
}

void
dumpAllocationInfo() {
	Logger::log(Logger::INFO, "memory: @bytes allocated(@times)", (int64_t)allocatedSize, (int16_t)allocateCount);
}
