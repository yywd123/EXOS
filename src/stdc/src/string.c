/*	String Functions for EXOS
 *	Copyright (C) 2020-2022 yywd_123
 *	Author:yywd_123
 *	Date:2022-5-27
*/

#include <EXOS.h>

void itol(int32_t Num, wchar_t *Result, wchar_t Mode)
{
  wchar_t *p = Result;
  wchar_t *p1, *p2;
  uint32_t Num_Unsigned = Num;
  int divisor = 10;

  if(Mode == L'd' && Num < 0)
  {
    *p++ = L'-';
    ++Result;
    Num_Unsigned = -Num;
  }
  else if(Mode == L'x') divisor = 16;

  int n = sizeof(int32_t) * 2;
  do
  {
    int remain = Num_Unsigned % divisor;
    *p++ = (remain < 10) ? remain + L'0' : remain + L'a' - 10;
    --n;
  }while(Num_Unsigned /= divisor);

  if(Mode == L'x') while(n--) *p++ = L'0';

  *p = 0;

  p1 = Result;
  p2 = p - 1;
  while(p1 < p2)
  {
    wchar_t tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    ++p1;
    --p2;
  }
}

