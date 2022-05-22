#include <EXOS.h>
#include <multiboot2.h>
#include <stdio.h>

#include <display/display.h>
#include <display/print.h>
#include <dev/memory/memory.h>
//#include <dev/media/disk/disk.h>
//#include <dev/keyboard/keyboard.h>

// structs
typedef struct
{
  uint8_t *CmdLine;
  uint8_t *LoaderName;
  struct multiboot_tag_module *ModInfo;
  struct multiboot_tag_mmap *MemMap;
} BootInfo;

// OSfunc.asm
//void io_hlt(void);
//void io_nop(void);
void Shutdown(void);
void Reboot(void);
