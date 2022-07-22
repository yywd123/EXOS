/*
  FrameBuffer Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2020-5-17
*/

#include <OSBase.h>

extern VideoInfo Vinfo;

void EXOSAPI VideoInit(BootInfo *info)
{
  Vinfo.fbAddress = info->Vinfo.fbAddress;
  Vinfo.fb = (uint32_t*)info->Vinfo.fbAddress;
  Vinfo.pitch = info->Vinfo.pitch;
  Vinfo.Screen_width = info->Vinfo.Screen_width;
  Vinfo.Screen_height = info->Vinfo.Screen_height;

  Vinfo.BackGround_Color = 0xff008080;
  Vinfo.ForeGround_Color = 0xffffffff;
  Vinfo.Cursor_x = 0;
  Vinfo.Cursor_y = 0;
}

void EXOSAPI DrawPixel(const uint32_t x, const uint32_t y, const uint32_t color)
{
  uint32_t *Pixel = Vinfo.fb + (Vinfo.pitch * y + x);
  *Pixel = color;
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
  DrawBlock(0, 0, Vinfo.Screen_height, Vinfo.Screen_width, Vinfo.BackGround_Color);
}

void EXOSAPI GraphicTest(void)
{
  wprintf(L"\n[ INFO ] Graphic Test !!!\n");
  wprintf(L"\n[ INFO ] Test 1: Color Test\n");
  for(uint32_t i = 0xff000000, x = 0, y = 0; i < 0xfffffffe; ++i)
  {
    DrawPixel(x++, y, i);
    if(x >= Vinfo.Screen_width)
    {
      x = 0;
      ++y;
    }

    if(y >= Vinfo.Screen_height) y = 0;
  }

  for (wchar_t i = 0; i < 0xffff; ++i)
  {
    wputc(i);
  }

  ClearScreen(NULL);

  wprintf(L"\n[ INFO ] Graphic Test Finished !!!\n");

  return;
}
