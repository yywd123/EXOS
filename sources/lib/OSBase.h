#include <stdint.h>
#include <stddef.h>
#include <multiboot2.h>

// structs
typedef struct
{
 
} Boot_Info;;

// OSfunc.asm
void io_hlt(void);
// Kernel.c
//void KernelInit(unsigned long addr);
// framebuffer.c
void *fb;
uint8_t bpp;
uint32_t pitch;
void DrawPixel(uint32_t x, uint32_t y, uint32_t color);

// function libs
#include <display/display.h>
