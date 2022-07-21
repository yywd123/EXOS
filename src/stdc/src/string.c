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

  do
  {
    int remain = Num_Unsigned % divisor;
    *p++ = (remain < 10) ? remain + L'0' : remain + L'a' - 10;
  }while(Num_Unsigned /= divisor);

  *p = 0;

  p1 = Result;
  p2 = p - 1;
  while(p1 < p2)
  {
    *p1 += *p2;
    *p2 = *p1 - *p2;
    *p1 -= *p2;
    ++p1;
    --p2;
  }
}

void EXOSAPI *memcpy(void *target, const void *source, size_t size)
{
  if(!target || !source || !size) return nullptr;
  uint32_t WordCount = size / 4;
  uint32_t Slice = size % 4;
  uint32_t *TargetPtr = (uint32_t *)target;
  uint32_t *SourcePtr = (uint32_t *)source;

  while(WordCount--) *TargetPtr++ = *SourcePtr++;
  while(Slice--) *((uint8_t *)TargetPtr++) = *((uint8_t *)SourcePtr++);

  return target;
}

void EXOSAPI *memset(void *target, const uint8_t data, size_t size)
{
  if(!target || !size) return nullptr;

  uint8_t *TargetPtr = (uint8_t *)target;

  while(--size) *TargetPtr++ = data;
  return target;
}

int EXOSAPI memcmp(const void *buf1, const void *buf2, size_t size)
{
  if(buf1 == nullptr && buf2 == nullptr) return 1;

  size_t i = 0;
  uint8_t *p1 = (uint8_t *)buf1;
  uint8_t *p2 = (uint8_t *)buf2;
  while(*p1++ == *p2++) ++i;

  if(i != size) return -1;

  return 0;
}
