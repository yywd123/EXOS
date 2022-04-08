/*
  Kernel for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2022-4-1
*/

#include <OSBase.h>

uint64_t Addr;
uint32_t magic;

int KernelInit(void);

int KernelInit(void)
{
  if(magic != 0xe85250d6 || Addr & 7) return -1;
  struct multiboot_tag *tag;
  for(tag = (struct multiboot_tag *)(Addr + 8);
      tag->type != 0;
      tag = (struct multiboot_tag *)(uint8_t *)tag + ((tag->size + 7) & ~ 7))
  {
    switch(tag->type)
    {
      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
          uint32_t mem_lower = ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower;
          uint32_t mem_upper = ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper;
        }
        break;
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
          tagfb = (struct multiboot_tag_framebuffer *)tag;
          Video_info Video;
          Video.VideoBuffer = (void *)(unsigned long) tagfb->common.framebuffer_addr;
          Video.bpp = tagfb->common.framebuffer_bpp;
          Video.pitch = tagfb->common.framebuffer_pitch;
          uint32_t color = 0;
          color = 0xff00ff00;
          for(unsigned i = 0; i < tagfb->common.framebuffer_width && i < tagfb->common.framebuffer_height; ++i) DrawPixel(Video, i, i, color);
        }
        break;
      default:
        break;
    }
  }
  return 0;
}
