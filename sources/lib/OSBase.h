#include <EXOS.h>
#include <multiboot2.h>


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
void Shutdown(void);
void Reboot(void);
void EXOSAPI outb(uint8_t data, uint16_t port);
void EXOSAPI outw(uint16_t data, uint16_t port);
void EXOSAPI outd(uint32_t data, uint16_t port);
uint8_t EXOSAPI inb(uint16_t port);
uint16_t EXOSAPI inw(uint16_t port);
uint32_t EXOSAPI ind(uint16_t port);
// Kernel.c
struct multiboot_tag *tag;
BootInfo BOOTINFO;
//void KernelInit(unsigned long addr);

// function libs
#include <display/display.h>
#include <window/window.h>
#include <io/io.h>
