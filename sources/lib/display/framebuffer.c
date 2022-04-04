/*
  FrameBuffer Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2020-4-3
*/

static uint8_t *Video;

static void DrawPixel(struct multiboot_tag_framebuffer *tagfb, unsigned x, unsigned y, uint32_t color);

static void DrawPixel(struct multiboot_tag_framebuffer *tagfb, unsigned x, unsigned y, uint32_t color)
{
  Video = (void *)(unsigned long)tagfb->common.framebuffer_addr;
  switch(tagfb->common.framebuffer_bpp)
  {
    case 8:
      *((uint8_t *)Video + tagfb->common.framebuffer_pitch * y + x) = color;      //Write color to uint8_t pointer to Video Memory
      break;
    case 15:
    case 16:
      *((uint16_t *)Video + tagfb->common.framebuffer_pitch * y + 2 * x)= color;  //Write color to uint16_t pointer to Video Memory
      break;
    case 24:
      *((uint32_t *)Video + tagfb->common.framebuffer_pitch * y + 3 * x) =        //Write color to uint32_t pointer to Video Memory(bpp:24)
        (color & 0xffffff) |
        (*((uint32_t *)Video + tagfb->common.framebuffer_pitch * y + 3 * x) & 0xff000000);
      break;
    case 32:
      *((uint32_t *)Video + tagfb->common.framebuffer_pitch * y + 3 * x) = color; //Write color to uint32_t pointer to Video Memory
      break;
  }
}
