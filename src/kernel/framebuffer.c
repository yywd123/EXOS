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

void EXOSAPI ClearScreen(const uint32_t color)
{
  if(color != NULL) Vinfo.BackGround_Color = color;
  DrawBlock(0, 0, Vinfo.Scrn_height, Vinfo.Scrn_width, Vinfo.BackGround_Color);
}

void EXOSAPI GraphicTest(void)
{
  wprintf(L"\n[ INFO ] Graphic Test !!!\n");
  wprintf(L"\n[ INFO ] Test 1: Color Test\n");
  for(uint32_t i = 0xff000000, x = 0, y = 0; i < 0xfffffffe; ++i)
  {
    DrawPixel(x++, y, i);
    if(x >= Vinfo.Scrn_width)
    {
      x = 0;
      ++y;
    }

    if(y >= Vinfo.Scrn_height) y = 0;
  }

  ClearScreen(NULL);

  wprintf(L"\n[ INFO ] Graphic Test Finished !!!\n");

  return;
}
