#include <stdint.h>
#include <stddef.h>
#include <multiboot2.h>

// variables
struct multiboot_tag *tag;
struct multiboot_tag_framebuffer *tagfb;
void *fb;

// function libs
#include <display/framebuffer.c>
