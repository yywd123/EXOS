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
  while(--size) *((uint8_t *)target)++ = *((uint8_t *)source)++;
  return target;
}

void EXOSAPI *memset(void *target, const uint8_t data, size_t size)
{
  if(!target || !size) return nullptr;
  while(--size) *((uint8_t *)target)++ = data;
  return target;
}
