/*
 *  Memory Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-4
*/

#include <OSBase.h>

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
