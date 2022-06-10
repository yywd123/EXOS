/*
  FrameBuffer Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2020-5-17
*/

#include <OSBase.h>

extern VideoInfo Vinfo;

void EXOSAPI DrawPixel(const uint32_t x, const uint32_t y, const uint32_t color)
{
  switch(Vinfo.bpp)
  {
    case 8:
      {
        uint8_t *Pixel = Vinfo.fb + Vinfo.pitch * y + x;
        *Pixel = color;
      }
      break;
    case 15:
    case 16:
      {
        uint16_t *Pixel = Vinfo.fb + Vinfo.pitch * y + 2 * x;
        *Pixel = color;
      }
      break;
    case 24:
      {
        uint32_t *Pixel = Vinfo.fb + Vinfo.pitch * y + 3 * x;
        *Pixel = (color & 0xffffff) | (*Pixel & 0xff000000);
      }
      break;
    case 32:
      {
        uint32_t *Pixel = Vinfo.fb + Vinfo.pitch * y + 4 * x;
        *Pixel = color;
      }
      break;
  }
}


void EXOSAPI DrawBlock(const uint32_t x, const uint32_t y, const uint32_t h, const uint32_t v, const uint32_t color)
{
  uint32_t x_off, y_off;
  for(y_off = 0;y_off <= h; ++y_off)
  {
    for(x_off = 0; x_off <= v; ++x_off) DrawPixel(x + x_off, y + y_off, color);
  }
}

