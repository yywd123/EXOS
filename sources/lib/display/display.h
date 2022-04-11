/*
  Display Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  date:2022-4-5
*/

typedef struct
{
  uint8_t bpp;
  uint32_t pitch;
  uint32_t Scrn_width;
  uint32_t Scrn_height;
  void *fb;
} Video_info;

Video_info Vinfo;

#include "framebuffer.c"
