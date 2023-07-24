#pragma once

#define PAGE_SIZE (0x1000)
#define PAGE_SHIFT (12)
#define ADDR_TO_PAGE_INDEX(addr) ((addr + PAGE_SIZE - 1) >> PAGE_SHIFT)

__NAMESPACE_DECL(Memory::Paging)

void __INIT
initialize(uint64_t pageCount);

__NAMESPACE_END