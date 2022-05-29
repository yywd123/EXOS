/*  Math Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-28
*/

#include <EXOS.h>

double log(double a)
{
  int n = 15;
  int k, nk;
  double x, xx, y;

  x= (a - 1) / (a + 1);
  xx = x * x;
  nk = 2 * n + 1;
  y = 1.0 / nk;

  for(k = n; k > 0; k--)
  {
    nk -= 2;
    y = 1.0 / nk + xx * y;
  }

  return 2.0 * x * y;
}
