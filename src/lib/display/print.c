/*
 *  Print Functions for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-9
*/

#include <OSBase.h>

extern VideoInfo Vinfo;

#define TextTypeBufferMAX 512

uint8_t TextTypeBuffer[TextTypeBufferMAX] = {0};

void ScrollScreen(const uint8_t rows)
{
  if(16 * rows <= Vinfo.Screen_height) Vinfo.Cursor_y -= 16 * rows;
  else    //clear screen
  {
    DrawBlock(0, 0, Vinfo.Screen_height, Vinfo.Screen_width, 0xff000000);
    return;
  }
  memcpy(Vinfo.fb, Vinfo.fb + 16 * rows * Vinfo.Screen_width * 4, Vinfo.Screen_width * (Vinfo.Screen_height - 16 * rows) * 4);
  DrawBlock(0, Vinfo.Screen_height - 16, 16, Vinfo.Screen_width, Vinfo.BackGround_Color);
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

void wputc(const wchar_t c)
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
  if(Vinfo.Screen_height <= Vinfo.Cursor_y) ScrollScreen(1);
  if(Vinfo.Screen_width - Vinfo.Cursor_x < font_width)
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

void wputs(const wchar_t *str)
{
  uint32_t c;
  while((c = *str++) != 0) wputc(c);
}

void wprintf(const wchar_t *format, ...)
{
  io_cli();
  va_list ap;
  va_start(ap, format);

  wchar_t c, buf[20] = {0};

  while((c = *format++) != 0)
  {
    if(c != L'%') wputc(c);
    else
    {
      c = *format++;

      // formats
      switch(c)
      {
        case L'd':
        case L'u':
        case L'x':
          {
            int num = va_arg(ap, int);
            itol(num, buf, c);
            wputs(buf);
          }break;

        case L's':
        {
          wchar_t *p = va_arg(ap, wchar_t *);
          wputs(p);
        }break;

        case 'c':
        {
          wchar_t p = va_arg(ap, wchar_t);
          wputc(p);
        }break;

        default:
        {
          wputc(*format++);
        }break;
      }
    }
  }
  
  va_end(ap);

  io_sti();
}
