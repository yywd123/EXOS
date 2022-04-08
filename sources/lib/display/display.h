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
  void *VideoBuffer;
} Video_info;

#include "framebuffer.c"
