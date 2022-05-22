/*
 * Disk I/O Library for EXOS
 * Copyright (C) 2020-2022 yywd_123
 * Author:yywd_123
 * Date:2022-5-6
*/

#include <OSBase.h>

void EXOSAPI ReadDisk_IDE(uint32_t LBA, void *Buffer, uint16_t SectorCount)
{
  outb(0x1f7, 0x0c);
  outw(0x1f2, SectorCount);                   // 1. Set SectorCount

  // 2. Set LBA Address
  outb(0x1f3, LBA & 0xff);
  outb(0x1f4, (LBA >> 8) & 0xff);
  outb(0x1f5, (LBA >> 16) & 0xff);
  outb(0x1f6, ((LBA >> 24) & 0xff) | 0xe0);

  // 3. Write Read Command
  outb(0x1f7, 0x20);
 
  // 4. Read Disk
  uint16_t WordCount = SectorCount * 256;

  while(WordCount--)
  {
    while((inb(0x1f7) & 0x88) != 0x08) printk(LOG_DEBUG, "HDD Waiting");    // Wait Disk
                                                   
    *((uint16_t *)Buffer + WordCount * 2) = inw(0x1f0);
    //((uint16_t)Buffer)++;
    //printk(LOG_INFO, "HDD Read a word");
  }
  printk(LOG_INFO, "HDD Read Success!!!");
}
