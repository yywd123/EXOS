void EXOSAPI *memcpy(void *target, const void *source, size_t size)
{
  if(!target | !source | !size) return NULL;
  while(--size) *(uint8_t *)++target = *(uint8_t *)++source;
  return target;
}

void EXOSAPI *memset(void *target, const uint8_t data, size_t size)
{
  if(!target | !size) return NULL;
  while(--size) *(uint8_t *)++target = data;
  return target;
}
