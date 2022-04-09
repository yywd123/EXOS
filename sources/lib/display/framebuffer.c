/*
  FrameBuffer Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2020-4-3
*/

void DrawPixel(uint32_t x, uint32_t y, uint32_t color)
{
  switch(bpp)
  {
    case 8:
      {
        uint8_t *Pixel = fb + pitch * y + x;
        *Pixel = color;
      }
      break;
    case 15:
    case 16:
      {
        uint16_t *Pixel = fb + pitch * y + 2 * x;
        *Pixel = color;
      }
      break;
    case 24:
      {
        uint32_t *Pixel = fb + pitch * y + 3 * x;
        *Pixel = (color & 0xffffff) | (*Pixel & 0xff000000);
      }
      break;
    case 32:
      {
        uint32_t *Pixel = fb + pitch * y + 4 * x;
        *Pixel = color;
      }
      break;
  }
}
