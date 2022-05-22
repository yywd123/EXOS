/*
 *  Print Functions for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-9
*/

#include <OSBase.h>

extern Video_Info Vinfo;

#define TextTypeBufferMAX 512

uint8_t TextTypeBuffer[TextTypeBufferMAX] = {0};

void ScrollScreen(const uint8_t rows)
{
  if(16 * rows <= Vinfo.Scrn_height) Vinfo.Cursor_y -= 16 * rows;
  else    //clear screen
  {
    DrawBlock(0, 0, Vinfo.Scrn_height, Vinfo.Scrn_width, 0xff000000);
    return;
  }
  memcpy(Vinfo.fb, Vinfo.fb + 16 * rows * Vinfo.Scrn_width * 4, Vinfo.Scrn_width * (Vinfo.Scrn_height - 16 * rows) * 4);
  memset(Vinfo.fb + Vinfo.Scrn_width * (Vinfo.Scrn_height - 16 * rows) * 4, 0, 16 * rows * Vinfo.Scrn_width * 4);
}

void backspace(const uint8_t n)
{
  if(TextTypeBuffer[Vinfo.Cursor_x / 16 - 1 ])
  { 
    Vinfo.Cursor_x -= 16;
    DrawBlock(Vinfo.Cursor_x, Vinfo.Cursor_y, 16, 16, 0xff000000);
  }
  else 
  {
    Vinfo.Cursor_x -= 8;
    DrawBlock(Vinfo.Cursor_x, Vinfo.Cursor_y, 16, 8, 0xff000000);
  }
}

void ClearTextTypeBuffer()
{
  for(int i = 0;TextTypeBuffer[i] != 0; ++i)
  {
    TextTypeBuffer[i] = 0;
  }
}

void putc(const long c)
{
  if(c == '\n') 
  {
    Vinfo.Cursor_y += 16;
    Vinfo.Cursor_x = 0;
    return;
  }
  if(c == '\b')
  {
    backspace(1);
    return;
  }

  uint8_t font_width = 16, font_height = 16, font_block_max = 2;
  if(Vinfo.Scrn_height <= Vinfo.Cursor_y) ScrollScreen(1);
  if(Vinfo.Scrn_width - Vinfo.Cursor_x < font_width)
  {
    Vinfo.Cursor_y += 16;
    Vinfo.Cursor_x = 0;
    ClearTextTypeBuffer();
  }

  uint32_t offset = c * 32;

  if(c <= 0x00ff) 
  {
    font_width = 8;
    font_block_max = 1; 
    TextTypeBuffer[Vinfo.Cursor_x / 16 - 1] = 0x2;
  }
  else TextTypeBuffer[Vinfo.Cursor_x / 16 - 1] = 0x1;
 
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

void puts(const long *str)
{
  uint32_t c;
  while((c = *str++) != 0) putc(c);
}

void printf(const long *format, ...)
{
  long **arg = (long **)&format;
  long c;

  while((c = *format++) != 0)
  {
    if(c != '%') putc(c);
    else
    {
      long *p;
      // formats
      switch(c)
      {
        case 'd':
        case 'u':
        case 'x':
          number:
            
        break;
        case 's':
          p = *arg++;
          while(*p) putc(*p++);
        break;

        default:
          goto number;
        break;

      }
    }
  }
}
