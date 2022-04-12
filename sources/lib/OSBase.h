#include <stdint.h>
#include <stddef.h>
#include <multiboot2.h>

// structs
typedef struct
{
  char ExitType;
  int StatCode;
} OSStat;

// OSfunc.asm
void io_hlt(void);
void *memcpy(void *target, void *source, uint32_t size);
// Kernel.c
struct multiboot_tag *tag;
//void KernelInit(unsigned long addr);
// framebuffer.c

// function libs
#include <display/display.h>
#include <window/window.h>
