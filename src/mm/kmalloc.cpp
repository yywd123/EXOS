#include <mm/kmalloc.hpp>
#include <efi/efi.hpp>
#include <exos/logger.hpp>
#include <exos/panic.hpp>

USE(EXOS::Utils);

__NAMESPACE_DECL(Memory::KMemory)

static BitArray pageAllocationStatus{nullptr, 0};
static PageBlock *mmap = nullptr;
static uint16_t mmapEntryCount = 0;

static bool
isPageRegionAllocated(uint32_t base, uint32_t count) {
	_iter(count) {
		if(pageAllocationStatus.get(base + i)) return true;
	}

	return false;
}

static void
setPageRegionAllocationStatus(uint32_t base, uint32_t count, bool allocated) {
	Logger::log(Logger::DEBUG, "setting page region allocation status: base @, count @, allocated @", base, count, allocated);
	_iter(count) {
		pageAllocationStatus.set(base + i, allocated);
	}
}

static uint16_t __INIT
getMergedMemoryMap(PageBlock **buffer) {
	uint64_t mapKey = 0;
	uint64_t orignalEntryCount = EFI::getMemoryMap((EFI::MemoryDescriptor **)buffer, &mapKey);

	uint16_t lastIndex = 0, mergedEntryCount = 0;
	for(uint16_t i = 0; i < orignalEntryCount; ++i) {
		if((*buffer)[i].type == BootServicesCode || (*buffer)[i].type == BootServicesData)
			(*buffer)[i].type = ConventionalMemory;
		(*buffer)[i].freePageCount = (*buffer)[i].pageCount;
		if(i != lastIndex) {
			if((*buffer)[i].type == (*buffer)[lastIndex].type) {
				(*buffer)[lastIndex].pageCount += (*buffer)[i].pageCount;
				(*buffer)[lastIndex].freePageCount += (*buffer)[i].freePageCount;
				continue;
			} else {
				__builtin_memcpy(&(*buffer)[++lastIndex], &(*buffer)[i], sizeof(PageBlock));
				++mergedEntryCount;
				continue;
			}
		}
		lastIndex = i;
		++mergedEntryCount;
	}

	return mergedEntryCount;
}

void
initialize() {
	mmapEntryCount = getMergedMemoryMap(&mmap);

	uint64_t freePageCount = 0;
	_iter(mmapEntryCount) {
		if(mmap[i].type == ConventionalMemory) freePageCount += mmap[i].pageCount;
	}
	mmap = nullptr;

	uint64_t allocationStatusSize = freePageCount;
	pageAllocationStatus = BitArray(EFI::allocatePool(__divceil(allocationStatusSize, 8)), allocationStatusSize);

	mmapEntryCount = getMergedMemoryMap(&mmap);
	uint64_t allocationBase = 0;
	_iter(mmapEntryCount) {
		if(mmap[i].pageCount && mmap[i].type == ConventionalMemory) {
			mmap[i].allocationBase = allocationBase;
			allocationBase += mmap[i].pageCount;
		}
		// Logger::log(Logger::DEBUG, "memory area @: start 0x@, size @ pages, type @, allocationBase @", i, mmap[i].physicalStart, (int64_t)mmap[i].pageCount, mmap[i].type, (int64_t)mmap[i].allocationBase);
	}

	setPageRegionAllocationStatus(0, 1, true);
}

void *
alloc(size_t size) {
}

void *
alloc4KPages(uintptr_t address, uint32_t count) {
	if(count == 0) return nullptr;

	address &= ~PAGE_4KMASK;
	_iter(mmapEntryCount) {
		if(mmap[i].physicalStart <= address && mmap[i].physicalStart + (mmap[i].pageCount << PAGE_4KSHIFT) >= address + (count << PAGE_4KSHIFT)) {
			if(mmap[i].type != ConventionalMemory) return nullptr;

			uint32_t base = mmap[i].allocationBase + ((address - mmap[i].physicalStart) >> PAGE_4KSHIFT);
			if(isPageRegionAllocated(base, count)) return nullptr;
			setPageRegionAllocationStatus(base, count, true);
			mmap[i].freePageCount -= count;
			return (void *)(mmap[i].physicalStart + ((base - mmap[i].allocationBase) << PAGE_4KSHIFT));
		}
	}

	return nullptr;
}

void *
alloc4KPages(uint32_t count) {
	if(count == 0) return nullptr;

	_iter(mmapEntryCount) {
		if(mmap[i].type != ConventionalMemory || count > mmap[i].freePageCount) continue;
		uint32_t currentBase = mmap[i].allocationBase;
		uint32_t currentSize = 0;
		__iter(j, mmap[i].pageCount) {
			if(!pageAllocationStatus.get(currentBase++)) {
				++currentSize;
			} else
				currentSize = 0;

			if(currentSize == count) {
				currentBase -= count;
				setPageRegionAllocationStatus(currentBase, count, true);
				mmap[i].freePageCount -= count;

				return (void *)(mmap[i].physicalStart + ((currentBase - mmap[i].allocationBase) << PAGE_4KSHIFT));
			}
		}
	}

	panic("out of memory");
	__UNREACHABLE;
}

void *
alloc2MPages(uint32_t count) {
	if(count == 0) return nullptr;

	_iter(mmapEntryCount) {
		uint32_t align = 512 - ((mmap[i].physicalStart & PAGE_2MMASK) >> PAGE_4KSHIFT);
		if(align == 512) align = 0;
		if(mmap[i].type != ConventionalMemory || align + count * 512 > mmap[i].freePageCount) continue;

		uint32_t currentBase = mmap[i].allocationBase + align;
		uint32_t currentSize = 0;

		__iter(j, (mmap[i].pageCount - align) / 512) {
			if(!isPageRegionAllocated(currentBase, 512)) {
				++currentSize;
			} else {
				currentSize = 0;
			}
			currentBase += 512;

			if(currentSize == count) {
				currentBase -= count * 512;

				setPageRegionAllocationStatus(currentBase, count * 512, true);
				mmap[i].freePageCount -= count * 512;

				return (void *)(mmap[i].physicalStart + ((currentBase - mmap[i].allocationBase) << PAGE_4KSHIFT));
			}
		}
	}

	panic("out of memory");
	__UNREACHABLE;
}

void
freePages(void *p, uint32_t count) {
	if(count == 0) return;

	uintptr_t address = (uintptr_t)p & ~PAGE_4KMASK;
	_iter(mmapEntryCount) {
		if(mmap[i].physicalStart <= address && mmap[i].physicalStart + (mmap[i].pageCount << PAGE_4KSHIFT) >= address + (count << PAGE_4KSHIFT)) {
			if(mmap[i].type != ConventionalMemory) return;

			uint32_t base = mmap[i].allocationBase + ((address - mmap[i].physicalStart) >> PAGE_4KSHIFT);
			setPageRegionAllocationStatus(base, count, false);
			mmap[i].freePageCount += count;
			return;
		}
	}
}

void
free(void *p) {
}

__NAMESPACE_END