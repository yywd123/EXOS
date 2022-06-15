/*  Bitmap Display Module for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-6-12
*/

#include "bmp.h"

extern VideoInfo Vinfo;

uint8_t EXOSMOD BmpDisplay(uint32_t x, uint32_t y, void *Dataptr)
{
  BmpInfo *Info = (BmpInfo *)&Dataptr;
  //if(Info->Magic != 0x4d42) return 1;
  //if(x + Info->BmpWidth > Vinfo.Scrn_width || y + Info->BmpHeight > Vinfo.Scrn_height) return 2;

  uint8_t *p = ((uint8_t *)&Dataptr + Info->DataOffset);
  uint8_t r, g, b;
  uint32_t color = 0;
  uint32_t BmpX, BmpY, X, Y;
  for(BmpY = 0; BmpY < Info->BmpHeight; ++BmpY)
  {
    for(BmpX = 0; BmpX < Info->BmpWidth; ++BmpX)
    {
      b = p[BmpY * 3 + BmpX * 3 + 0];
      g = p[BmpY * 3 + BmpX * 3 + 1];
      r = p[BmpY * 3 + BmpX * 3 + 2];

      color = 0xff000000 + r * 0x10000 + g * 0x100 + b;

      X = x + BmpX;
      Y = y + Info->BmpHeight - 1 - BmpY;
      DrawPixel(X, Y, color);
    }
  }

  return 0;
}
