#include <efi/efi.hpp>
#include <mm/MemoryAllocator.hpp>
#include <exos/logger.hpp>

#define InitHeapPageCount 16

USE(EXOS::Memory);
USE(EXOS::Utils);

MemoryAllocator allocator;

class InitHeapAllocator : public AbstractMemoryAllocator {
private:
	uint8_t *current = nullptr;
	uint8_t *end = nullptr;

public:
	void *
	alloc(size_t size) {
		if(current + size > end) {
			return nullptr;
		}
		void *p = current;
		current += size;
		return p;
	}

	void
	free(void *p) {
	}

	InitHeapAllocator(uint8_t *heap, size_t heapSize) {
		current = heap + sizeof(*this);
		end = heap + heapSize;
	}

	__PLACEMENTNEW_DEFAULT
};

void __INIT
initializeInitHeapAllocator() {
	uint8_t *heap = (uint8_t *)EFI::allocatePages(InitHeapPageCount);
	allocator = new(heap) InitHeapAllocator(heap, InitHeapPageCount * EFI_PAGE_SIZE);
}

void __INIT
disableInitHeap(MemoryAllocator newAllocator) {
	void *initHeap = (void *)allocator;
	allocator = newAllocator;

	//将initheap标记为占用
}

static size_t allocatedSize = 0;
static uint16_t allocateCount = 0;

void *
operator new(size_t size) {
	Logger::log(Logger::INFO, "memory alloc: @bytes", (int64_t)size);
	++allocateCount;
	allocatedSize += size;
	return EFI::allocatePool(size);
	// return allocator->alloc(size);
}

void *
operator new[](size_t size) {
	Logger::log(Logger::INFO, "memory alloc: @bytes", (int64_t)size);
	++allocateCount;
	allocatedSize += size;
	return EFI::allocatePool(size);
	// return allocator->alloc(size);
}

void
operator delete(void *p) {
	//--allocateCount;
	EFI::freePool(p);
	// allocator->free(p);
}

void
operator delete(void *p, size_t) {
	//--allocateCount;
	EFI::freePool(p);
	// allocator->free(p);
}

void
dumpAllocationInfo() {
	Logger::log(Logger::INFO, "memory: @bytes allocated(@times)", (int64_t)allocatedSize, (int16_t)allocateCount);
}
