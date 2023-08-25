#include <mm/paging.hpp>
#include <mm/kmalloc.hpp>
#include <platform/platform.hpp>
#include <exos/logger.hpp>

USE(EXOS::Utils);

__NAMESPACE_DECL(Memory::Paging)

static bool is5LevelPagingSupport = false;
static uintptr_t kernelPageMap = 0;
static uint64_t kernelPageDirectoryCount = 0;

//  支持5级分页就是pml5 不然是pml4
static uint64_t *pageMapLevelMax = nullptr;

void *
getPageLevelMax() {
	return (void *)pageMapLevelMax;
}

uint64_t
getCanonicalAddressMask() {
	return is5LevelPagingSupport ? 0x1ffffffffffffff : 0xffffffffffff;
}

uint64_t *
getPageDirectory(bool kAddressSpace, uint32_t index) {
	if(kAddressSpace && index >= kernelPageDirectoryCount) return nullptr;
	uint64_t *pml4Entry = nullptr;

	if(is5LevelPagingSupport) {
		uint64_t *pml5Entry = &((uint64_t *)kernelPageMap)[kAddressSpace ? 0 : 0x1ff];
		if(!(*pml5Entry & PAGE_P)) return nullptr;
		pml4Entry = (uint64_t *)(*pml5Entry & PT_ADDRMASK);
	} else {
		pml4Entry = &((uint64_t *)kernelPageMap)[kAddressSpace ? 0 : 0x1ff];
		if(!(*pml4Entry & PAGE_P)) return nullptr;
	}

	uint64_t *pdpEntry = (uint64_t *)(pml4Entry[index >> 18] & PT_ADDRMASK);
	uint64_t *pdEntry = (uint64_t *)(pdpEntry[index >> 9] & PT_ADDRMASK);
	return &pdEntry[index & 0x1ff];
}

void __INIT
initialize() {
	uint8_t physicalAddressBits = 0;

	uint32_t pml5EntryCount = 1;
	uint32_t pml4EntryCount = 1;
	uint32_t pdpEntryCount = 1;
	uint32_t kernelPageTableSize = 0;

	uint32_t tmp;
	uint32_t eax, ecx;
	__cpuid(0x7, tmp, tmp, ecx, tmp);

	is5LevelPagingSupport = (ecx & BIT(16) != 0);
	// Logger::log(Logger::DEBUG, "pml5 support = @", is5LevelPagingSupport);

	__cpuid(0x80000000, eax, tmp, tmp, tmp);
	if(eax >= 0x80000008) {
		__cpuid(0x80000008, eax, tmp, tmp, tmp);
		physicalAddressBits = (uint8_t)eax;
	} else
		physicalAddressBits = 36;

	// Logger::log(Logger::DEBUG, "physcal address size: @", (int8_t)physicalAddressBits);

	if(physicalAddressBits > 48) {
		if(is5LevelPagingSupport) pml5EntryCount = BIT(physicalAddressBits - 48);
		physicalAddressBits = 48;
	}

	if(physicalAddressBits > 39) {
		pml4EntryCount = BIT(physicalAddressBits - 39);
		physicalAddressBits = 39;
	}

	pdpEntryCount = BIT(physicalAddressBits - 30);
	kernelPageTableSize = ((pdpEntryCount + 1) * pml4EntryCount + 1) * pml5EntryCount + (is5LevelPagingSupport ? 1 : 0);
	kernelPageDirectoryCount = ((pdpEntryCount)*pml4EntryCount) * pml5EntryCount * 512;

	void *kernelPageTable = KMemory::alloc4KPages(0x2000, kernelPageTableSize);
	uintptr_t pageTableAddress = (uintptr_t)kernelPageTable;
	kernelPageMap = pageTableAddress;

	// Logger::log(Logger::DEBUG, "paging: pml5: @, pml4: @, pdp: @", (int32_t)pml5EntryCount, (int32_t)pml4EntryCount, (int32_t)pdpEntryCount);
	// Logger::log(Logger::DEBUG, "paging: ptsize: @pages, pt: @", (int32_t)kernelPageTableSize, kernelPageTable);

	uint64_t *pml5Entry = nullptr;
	uint64_t *pml4Entry = nullptr;
	uint64_t *pdpEntry = nullptr;
	uint64_t *pdEntry = nullptr;
	uintptr_t pageAddress = 0;

	if(is5LevelPagingSupport) {
		pml5Entry = (uint64_t *)pageTableAddress;
		pageTableAddress += PAGE_4K;
	}

	__iter(pml5Index, pml5EntryCount) {
		pml4Entry = (uint64_t *)pageTableAddress;
		pageTableAddress += PAGE_4K;

		if(is5LevelPagingSupport) {
			*pml5Entry++ = (uint64_t)pml4Entry | PAGE_P | PAGE_RW;
		}

		__iter(pml4Index, pml5EntryCount == 1 ? pml4EntryCount : 512) {
			pdpEntry = (uint64_t *)pageTableAddress;
			pageTableAddress += PAGE_4K;

			*pml4Entry++ = (uint64_t)pdpEntry | PAGE_P | PAGE_RW;

			__iter(pdpIndex, pml4EntryCount == 1 ? pdpEntryCount : 512) {
				pdEntry = (uint64_t *)pageTableAddress;
				pageTableAddress += PAGE_4K;

				*pdpEntry++ = (uint64_t)pdEntry | PAGE_P | PAGE_RW;

				__iter(pdIndex, 512) {
					*pdEntry++ = (uint64_t)pageAddress | PAGE_P | PAGE_RW | BIT(7);
					pageAddress += PAGE_2M;
				}
			}
		}
	}

	ASM("mov %0, %%cr3" ::"r"(kernelPageMap));
}

void
refreshPageTable() {
	ASM("movq %cr3, %rax\n\t"
			"movq %rax, %cr3");
}

void
invalidateTLB(void *page) {
	ASM("invlpg (%0)" ::"r"(page));
}

bool
spiltPageDirectory(bool kAddressSpace, uint32_t index) {
	uint64_t *pdEntry = getPageDirectory(kAddressSpace, index);

	if(!pdEntry) return false;
	if(!checkFlag(*pdEntry, BIT(7))) return true;

	uintptr_t pageAddress = *pdEntry & PT_ADDRMASK;
	uint64_t *ptEntry = (uint64_t *)KMemory::alloc4KPages(1);
	_iter(512) {
		ptEntry[i] = pageAddress + (i << PAGE_4KSHIFT) | PAGE_P | PAGE_RW | BIT(7);
	}

	*pdEntry = (*pdEntry & ~(PT_ADDRMASK | BIT(7))) | (uint64_t)ptEntry;
	invalidateTLB((void *)pageAddress);

	return true;
}

bool
mergePageDirectory(bool kAddressSpace, uint32_t index) {
	uint64_t *pdEntry = getPageDirectory(kAddressSpace, index);

	if(!pdEntry) return false;
	if(checkFlag(*pdEntry, BIT(7))) return true;

	uint64_t *ptEntry = (uint64_t *)(*pdEntry & PT_ADDRMASK);
	uintptr_t pageAddress = ptEntry[0] & PT_ADDRMASK;

	*pdEntry = (*pdEntry & ~(PT_ADDRMASK)) | pageAddress | BIT(7);
	KMemory::freePages(ptEntry, 1);

	invalidateTLB((void *)pageAddress);

	return true;
}

bool
map4KPage(uintptr_t paddr, uintptr_t vaddr, uint64_t flags) {
	bool addressSpace = !checkFlag(vaddr, BIT(63));
	uint32_t pdIndex = (vaddr & getCanonicalAddressMask()) >> PAGE_2MSHIFT;
	uint64_t *pdEntry = getPageDirectory(addressSpace, pdIndex);

	if(!pdEntry) return false;
	if(checkFlag(*pdEntry, BIT(7))) {
		spiltPageDirectory(addressSpace, pdIndex);
	}

	uint32_t ptIndex = (vaddr & PAGE_2MMASK) >> PAGE_4KSHIFT;
	uint64_t *ptEntry = (uint64_t *)(*pdEntry & PT_ADDRMASK);

	ptEntry[ptIndex] = (flags & ~(PT_ADDRMASK)) | paddr | BIT(7);

	invalidateTLB((void *)vaddr);

	return true;
}

bool
map2MPage(uintptr_t paddr, uintptr_t vaddr, uint64_t flags) {
	if((paddr & PAGE_2MMASK) || (vaddr & PAGE_2MMASK)) return false;
	bool addressSpace = !checkFlag(vaddr, BIT(63));
	uint32_t pdIndex = (vaddr & getCanonicalAddressMask()) >> PAGE_2MSHIFT;
	uint64_t *pdEntry = getPageDirectory(addressSpace, pdIndex);

	if(!pdEntry) return false;
	if(checkFlag(*pdEntry, BIT(7))) {
		*pdEntry = (flags & ~PT_ADDRMASK) | paddr | BIT(7);
	} else {
		uint64_t *ptEntry = (uint64_t *)(*pdEntry & PT_ADDRMASK);
		_iter(512) {
			ptEntry[i] = (flags & ~PT_ADDRMASK) | paddr + (i << PAGE_4KSHIFT) | BIT(7);
		}
	}

	invalidateTLB((void *)vaddr);

	return true;
}

bool
mapPages(uintptr_t paddr, uintptr_t vaddr, uint64_t pageCount, uint64_t flags) {
	bool addressSpace = !checkFlag(vaddr, BIT(63));
	uint32_t pdIndex = (vaddr & getCanonicalAddressMask()) >> PAGE_2MSHIFT;
	if(!getPageDirectory(addressSpace, pdIndex + (pageCount + 511) / 512)) return false;

	while(pageCount) {
		if((vaddr & PAGE_2MMASK) >> PAGE_4KSHIFT) {
		_:
			map4KPage(paddr, vaddr, flags);
			paddr += PAGE_4K;
			vaddr += PAGE_4K;
			--pageCount;
		} else {
			if(pageCount >= 512) {
				if((paddr & PAGE_2MMASK) >> PAGE_4KSHIFT) goto _;	 //	映射2mb大页 物理地址和虚拟地址都要2mb对齐 任意一个地址没有2mb对齐就4kb映射
				map2MPage(paddr, vaddr, flags);
				paddr += PAGE_2M;
				vaddr += PAGE_2M;
				pageCount -= 512;
			} else {
				goto _;
			}
		}
	}

	return true;
}

__NAMESPACE_END