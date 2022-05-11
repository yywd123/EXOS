#include <EXOS.h>
#include <multiboot2.h>

#include <dev/io/io.h>
#include <debug/debug.h>
#include <dev/memory/memory.h>
#include <dev/media/disk/disk.h>

// structs
typedef struct
{
  uint8_t *CmdLine;
  uint8_t *LoaderName;
  struct multiboot_tag_module *ModInfo;
  struct multiboot_tag_mmap *MemMap;
} BootInfo;

// OSfunc.asm
void io_hlt(void);
void io_nop(void);
void Shutdown(void);
void Reboot(void);

// Kernel.c
struct multiboot_tag *tag;
BootInfo BOOTINFO;
//void KernelInit(unsigned long addr);

// function libs
#include <display/display.h>

// syslib
#include <stdio.h>
