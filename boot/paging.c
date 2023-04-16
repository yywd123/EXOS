#include <utils.h>

#define FLAG_P 0b1
#define FLAG_RW 0b10
#define FLAG_USER 0b100
#define FLAG_2MB 0b10000000

#define PAGE_2M_MASK (~(0x1fffff))
#define PAGE_2M_SHIFT	21

static void setPageTableEntry(EFI_PHYSICAL_ADDRESS table, uint16_t i, EFI_PHYSICAL_ADDRESS addr, uint64_t attr) {
  ((uint64_t*)table)[i] = addr | attr;
}

uint64_t hugePageCount;
uint64_t ptCount = 512;
uint64_t pdCount = 512;

EFI_PHYSICAL_ADDRESS pdpt;

EFI_PHYSICAL_ADDRESS createPageTable(uint64_t pageCount) {
  Print(L"pageCount 0x%lx\n", pageCount);
  hugePageCount = pageCount * EFI_PAGE_SIZE / 0x200000;
  uint64_t pageTableSize = 1 + pdCount + ptCount;
  Print(L"ptCount %lu pdCount %lu pageTableSize %lu\n", ptCount, pdCount, pageTableSize);
  
  call(gBS->AllocatePages, AllocateAnyPages, EfiLoaderData, pageTableSize, &pdpt);
  call(gBS->SetMem, (void*)pdpt, pageTableSize * EFI_PAGE_SIZE, 0);

  for (uint64_t i = 0; i < pdCount; ++i) {
    setPageTableEntry(pdpt, i, pdpt + EFI_PAGE_SIZE * (i + 1), FLAG_RW | FLAG_P);
  }

  for (uint64_t i = 0; i < ptCount; ++i) {
    setPageTableEntry(pdpt + EFI_PAGE_SIZE, i, pdpt + EFI_PAGE_SIZE * (i + 1 + pdCount), FLAG_RW | FLAG_P);
  }

  for(uint64_t i = 0; i < hugePageCount; i++) {
    setPageTableEntry(pdpt + EFI_PAGE_SIZE * (pdCount + 1), i, i * 0x200000, FLAG_RW | FLAG_P | FLAG_2MB);
  }

  return pdpt;
}

static uint64_t *getPageTableEntryFromAddress(EFI_VIRTUAL_ADDRESS addr) {
  uint64_t *entries = (uint64_t*)(pdpt + (pdCount + 1) * EFI_PAGE_SIZE);
  if ((addr >> PAGE_2M_SHIFT) > hugePageCount) return NULL;
  return &entries[addr >> PAGE_2M_SHIFT];
}

BOOLEAN mapMemory(EFI_PHYSICAL_ADDRESS src, EFI_VIRTUAL_ADDRESS dest, UINT64 n) {
  Print(L"get 0x%lx\n", *getPageTableEntryFromAddress(dest));
  uint64_t *destPT = getPageTableEntryFromAddress(dest);
  if (!destPT) return FALSE;
  for (uint64_t i = 0; i < n; ++i) {
    uint64_t destEntry = destPT[i];
    destPT[i] = (destEntry & EFI_PAGE_MASK) | src;
  }
  
  asm("movq %cr3, %rax\n\tmovq %rax, %cr3");
  return TRUE;
}