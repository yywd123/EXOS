#include <multiboot2.h>   //always first

// variable
struct multiboot_tag_basic_meminfo *meminfo;
struct multiboot_tag_framebuffer *fbtag;
struct multiboot_tag_vbe *vbetag;
struct vbe_info_block *vbe_info;
struct vbe_mode_info_block *vbe_modeinfo;

// typedef
typedef struct 
{
   
}boot_info;

// function libs
#include <display/framebuffer.c>
