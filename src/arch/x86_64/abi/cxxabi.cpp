#include <efi/efi.hpp>
#include <mm/MemoryAllocator.hpp>

#define InitHeapPageCount 16

USE(EXOS::Memory);

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
	uint8_t *heap = (uint8_t *)efiAllocatePages(InitHeapPageCount);
	allocator = new(heap) InitHeapAllocator(heap, InitHeapPageCount * EFI_PAGE_SIZE);
}

void __INIT
disableInitHeap(MemoryAllocator newAllocator) {
	void *initHeap = (void *)allocator;
	allocator = newAllocator;

	//将initheap标记为占用
}

void *
operator new(size_t size) {
	return efiAllocatePool(size);
	// return allocator->alloc(size);
}

void *
operator new[](size_t size) {
	return efiAllocatePool(size);
	// return allocator->alloc(size);
}

void
operator delete(void *p) {
	efiFreePool(p);
	// allocator->free(p);
}

void
operator delete(void *p, size_t) {
	efiFreePool(p);
	// allocator->free(p);
}
