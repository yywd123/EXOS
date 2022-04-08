/*
  FrameBuffer Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2020-4-3
*/

void *VideoBuffer;

static void DrawPixel(Video_info tagfb, unsigned x, unsigned y, uint32_t color);

static void DrawPixel(Video_info tagfb, unsigned x, unsigned y, uint32_t color)
{
  switch(tagfb.bpp)
  {
    case 8:
      {
        uint8_t *Pixel8 = tagfb.VideoBuffer + tagfb.pitch * y + x;          //Init uint8_t pointer to VideoBuffer(bpp:8)
        *Pixel8 = color;        //Write pixel
      }
      break;
    case 15:
    case 16:
      {
        uint16_t *Pixel16 = tagfb.VideoBuffer + tagfb.pitch * y + 2 * x;  //Init uint16_t pointer to VideoBuffer(bpp:16)
        *Pixel16 = color;       //Write pixel
      }
      break;
    case 24:
      {
        uint32_t *Pixel24 = tagfb.VideoBuffer + tagfb.pitch * y + 3 * x;   //Init uint32_t pointer to VideoBuffer(bpp:24)
        *Pixel24 = (color & 0xffffff) | (*Pixel24 & 0xff00000000);        //Write pixel
      }
      break;
    case 32:
      {
        uint32_t *Pixel32 = tagfb.VideoBuffer + tagfb.pitch * y + 4 * x;  //Init uint32_t pointer to VideoBuffer(bpp:32)
        *Pixel32 = color;       //Write pixel
      }
      break;
  }
}
