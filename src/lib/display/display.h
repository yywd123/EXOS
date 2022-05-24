/*
  Display Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date:2022-5-4
*/

typedef struct
{
  uint8_t bpp;
  uint32_t pitch;
  uint32_t Scrn_width;
  uint32_t Scrn_height;
  uint32_t BackGround_Color;   // Black
  uint32_t ForeGround_Color;   // White
  void *fb;
  uint32_t Cursor_x;
  uint32_t Cursor_y;
} VideoInfo;

void EXOSAPI DrawPixel(const uint32_t x, const uint32_t y, const uint32_t color);
void EXOSAPI DrawBlock(const uint32_t x, const uint32_t y, const uint32_t h, const uint32_t v, const uint32_t color);
