/*
  Display Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date:2022-5-4
*/

#include "font.h"

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
} Video_info;

Video_info Vinfo;

#include "framebuffer.c"
