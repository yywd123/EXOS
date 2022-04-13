/*
  Windows Library for EXOS
  Copyright (C) 2020-2022 yywd_123
  Author:yywd_123
  Date:2022-4-10
*/

typedef struct
{
  uint32_t wid;       //Window ID
  uint8_t WindowName;
  uint8_t WindowType;
  uint32_t x,y,h,v;     //Window pos and size
  uint32_t IFArea_x,IFArea_y,IFArea_h,IFArea_v;   //MainInterface pos and size
} WINDOW;

#include "window.c"
