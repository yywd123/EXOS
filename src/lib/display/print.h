/*  Print Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-17
*/

#include "font.h"         //Font Library

void ScrollScreen(const uint8_t rows);
void backspace(const uint8_t n);
void ClearTextTypeBuffer();
void wputc(const wchar_t c);
void wputs(const wchar_t *str);
void wprintf(const wchar_t *format, ...);
