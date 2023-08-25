#pragma once

#define PAGE_4K (0x1000)
#define PAGE_4KSHIFT (12)
#define PAGE_4KMASK (0xfff)
#define PAGE_2M (0x200000)
#define PAGE_2MSHIFT (21)
#define PAGE_2MMASK (0x1fffff)

#define PAGE_P BIT(0)
#define PAGE_RW BIT(1)
#define PAGE_US BIT(2)
#define PAGE_WT BIT(3)
#define PAGE_CD BIT(4)
#define PAGE_AC BIT(5)
#define PAGE_DIRTY BIT(6)		//  仅在PD/PTE中有效
#define PAGE_PAT BIT(7)			//  仅在PD/PTE中有效
#define PAGE_GLOBAL BIT(8)	//  仅在PD/PTE中有效
#define PAGE_NX BIT(63)

#define PT_ADDRMASK (0x000ffffffffff000)

__NAMESPACE_DECL(Memory::Paging)

void *
getPageLevelMax();

uint64_t
getCanonicalAddressMask();

uint64_t *
getPageDirectory(bool kAddressSpace, uint32_t index);

void __INIT
initialize();

void
refreshPageTable();

void
invalidateTLB(void *page);

bool
spiltPageDirectory(bool kAddressSpace, uint32_t index);

bool
mergePageDirectory(bool kAddressSpace, uint32_t index);

/**
 * @brief	将一个4kb的物理页映射到虚拟地址
 * @param paddr	物理页地址(自动4k对齐)
 * @param vaddr	虚拟页地址(自动4k对齐)
 * @param flags	映射页面的标志
 * @return 映射是否成功
 */
bool
map4KPage(uintptr_t paddr, uintptr_t vaddr, uint64_t flags);

/**
 * @brief	将一个2mb的物理页映射到虚拟地址
 * @param paddr	物理页地址(强制2m对齐,否则失败)
 * @param vaddr	虚拟页地址(强制2m对齐,否则失败)
 * @param flags	映射页面的标志
 * @return 映射是否成功
 */
bool
map2MPage(uintptr_t paddr, uintptr_t vaddr, uint64_t flags);

/**
 * @brief 将物理地址映射到虚拟地址
 * @param paddr	物理地址(自动4k对齐)
 * @param vaddr 虚拟地址(自动4k对齐)
 * @param pageCount 映射的页数(4kb页)
 * @param flags	映射页面的标志
 * @return 映射是否成功
 */
bool
mapPages(uintptr_t paddr, uintptr_t vaddr, uint64_t pageCount, uint64_t flags);

__NAMESPACE_END