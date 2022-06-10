/*  Print Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-17
*/

#include "font.h"         //Font Library

void ScrollScreen(const uint8_t rows);
void backspace(const uint8_t n);
void ClearTextTypeBuffer();
void wputc(const long c);
void wputs(const long *str);
void wprintf(const long *format, ...);
