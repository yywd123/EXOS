/*
  FrameBuffer Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date: 2020-4-3
*/

void DrawPixel(uint32_t x, uint32_t y, uint32_t color)
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


void DrawBlock(const uint32_t x, const uint32_t y, const uint32_t h, const uint32_t v, const uint32_t color)
{
  uint32_t x_off, y_off;
  for(y_off = 0;y_off <= h; ++y_off)
  {
    for(x_off = 0; x_off <= v; ++x_off){
    switch(Vinfo.bpp)
    {
      case 8:
        {
          uint8_t *Pixel = Vinfo.fb + Vinfo.pitch * (y + y_off) + (x + x_off);
          *Pixel = color;
        }
        break;
      case 15:
      case 16:
        {
          uint16_t *Pixel = Vinfo.fb + Vinfo.pitch * (y + y_off) + 2 * (x + x_off);
          *Pixel = color;
        }
        break;
      case 24:
        {
          uint32_t *Pixel = Vinfo.fb + Vinfo.pitch * (y + y_off) + 3 * (x + x_off);
          *Pixel = (color & 0xffffff) | (*Pixel & 0xff000000);
        }
        break;
      case 32:
        {
          uint32_t *Pixel = Vinfo.fb + Vinfo.pitch * (y + y_off) + 4 * (x + x_off);
          *Pixel = color;
        }
        break;
      }
    }
  }
}

void putc(uint16_t c)
{
  if(c == '\n') 
  {
    Vinfo.Cursor_y + 16;
    return;
  }
  if(c == '\r')
  {
    Vinfo.Cursor_x - 8;
    return;
  }
  uint32_t offset = c * 32;

  uint8_t font_width = 16, font_height = 16, font_block_max = 2;
  if(c <= 0x00ff) 
  {
    font_width = 8;
    font_block_max = 1;
  }
  int font_x, font_y, font_block;
  for(font_y = 0; font_y < font_height; ++font_y)
  {
    for(font_block = 0; font_block < font_block_max; ++font_block)
    {
      for(font_x = 0; font_x < font_width; ++font_x)
      {
        if(UNICODE_16X16[offset + font_y * 2 + font_block] & (0x80 >> font_x)) 
          DrawPixel(Vinfo.Cursor_x + 8 * font_block + font_x, Vinfo.Cursor_y + font_y, Vinfo.ForeGround_Color);
        else DrawPixel(Vinfo.Cursor_x + 8 * font_block + font_x, Vinfo.Cursor_y + font_y, Vinfo.BackGround_Color);
      }
    }
  }
  Vinfo.Cursor_x += font_width;
}
