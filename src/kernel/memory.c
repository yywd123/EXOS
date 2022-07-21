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

uint32_t EXOSAPI GetLineAddress(uint32_t Table, uint32_t Page, uint32_t Offset)
{
  return (Table << 22) + (Page << 12) + Offset;
}