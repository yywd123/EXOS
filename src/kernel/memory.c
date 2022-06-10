/*  Memory Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-6-6
*/

#include <OSBase.h>

#define PGD_Addr 0x400000
#define PUD_Addr 0x401000
#define PMD_Addr 0x402000
#define PTE_Addr 0x403000
#define Page_Size 0x1000
#define Page_Addr (PTE_Addr + 0x400000)

#define PG_P 1
#define PG_USU 4
#define PG_RWW 2

extern BootInfo BOOTINFO;

static PAGE_INFO *Pages = (PAGE_INFO *)Page_Addr;

MMAP EXOSAPI MMap_Init(void)
{
  MMAP MemMap;
  multiboot_memory_map_t *mmap;
  uint16_t MemBlockCount = 0, AvailableMemBlockCount = 0;

  MemMap.MemAvailable = 0;
  MemMap.MemTotal = 0;
  
  wprintf(L"Memory Map:\n");
  for(mmap = BOOTINFO.MemMap->entries; 
      (uint8_t *)mmap < (uint8_t *)BOOTINFO.MemMap + BOOTINFO.MemMap->size;
       mmap = (multiboot_memory_map_t *)((unsigned long) mmap + sizeof(multiboot_memory_map_t)))
  {
    if(mmap->type) 
    {
      MemMap.MemAvailable += mmap->len / (1024 * 1024);     //1024byte * 1024 = 1MiB
      MemMap.block[AvailableMemBlockCount].Base_Addr = BOOTINFO.MemMap->entries->addr;
      MemMap.block[AvailableMemBlockCount].BlockSize = BOOTINFO.MemMap->entries->len;
      ++AvailableMemBlockCount;
    }

    MemMap.MemTotal += mmap->len / 1024;
    wprintf(L"  Block %d, Base_Addr: 0x%x%x, Size: 0x%x%x, Type: 0x%x\n", 
        MemBlockCount,
        (unsigned)(mmap->addr >> 32), (unsigned)(mmap->addr & 0xffffffff),
        (unsigned)(mmap->len >> 32), (unsigned)(mmap->len & 0xffffffff),
        (unsigned)(mmap->type));
    ++MemBlockCount;
  }

  MemMap.MemTotal /= 1024;

  return MemMap;
}

void EXOSAPI Page_Init(void)
{
  wprintf(L"\n Starting Init Page\n");
  memset(Pages, 0, 1024 * 1024);
  memset((void *)PGD_Addr, 0, Page_Addr - PGD_Addr);

  uint32_t addr, i;

  // Init PGD
  for(addr = PGD_Addr, i = PUD_Addr | PG_P | PG_USU | PG_RWW;
      addr < PUD_Addr; addr += 4, i += Page_Size)
  {
    *((uint32_t *)addr) = i;
  }
  // Init PUD
  for(addr = PUD_Addr, i = PMD_Addr | PG_P | PG_USU | PG_RWW;
      addr < PMD_Addr; addr += 4, i += Page_Size)
  {
    *((uint32_t *)addr) = i;
  }
  // Init PMD
  for(addr = PMD_Addr, i = PTE_Addr | PG_P | PG_USU | PG_RWW;
      addr < PTE_Addr; addr += 4, i += Page_Size)
  {
    *((uint32_t *)addr) = i;
  }

  // Init PTE
  for(addr = PTE_Addr, i = PG_P | PG_USU | PG_RWW;
      addr < Page_Addr; addr += 4, i += Page_Size)
  {
    *((uint32_t *)addr) = i;
  }

  // 0 ~ 0x1000
  Pages[0].Flag = 1;
  // 0x7000 ~ 0x8000
  Pages[7].Flag = 1;
  // 0x80000 ~ 0x901000
  for(i = 0x80000 / Page_Size; i < 0x901000 / Page_Size; ++i) Pages[i].Flag = 1;
  // 0xc0000000 ~ 0xffffffff
  for(i = 0xc0000000 / Page_Size; i < 0xffffffff / Page_Size; ++i) Pages[i].Flag = 1;

  __asm__ __volatile__("movl %%eax, %%cr3       \n\t"
                       "movl %%cr0, %%eax       \n\t"
                       "orl $0x80000000, %%eax  \n\t"
                       "movl %%eax, %%cr0       \n\t"
                       :: "a" (PGD_Addr));

  wprintf(L"\nawa?\n");
  return;
}

uint32_t EXOSAPI GetLineAddress(uint32_t Table, uint32_t Page, uint32_t Offset)
{
  return (Table << 22) + (Page << 12) + Offset;
}

void EXOSAPI *memcpy(void *target, const void *source, size_t size)
{
  if(!target || !source || !size) return nullptr;
  uint32_t WordCount = size / 4;
  uint32_t Slice = size % 4;
  uint32_t *TargetPtr = (uint32_t *)target;
  uint32_t *SourcePtr = (uint32_t *)source;

  while(WordCount--) *TargetPtr++ = *SourcePtr++;
  while(Slice--) *((uint8_t *)TargetPtr++) = *((uint8_t *)SourcePtr++);

  return target;
}

void EXOSAPI *memset(void *target, const uint8_t data, size_t size)
{
  if(!target || !size) return nullptr;

  uint8_t *TargetPtr = (uint8_t *)target;

  while(--size) *TargetPtr++ = data;
  return target;
}
