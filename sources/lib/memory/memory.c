void EXOSAPI *memcpy(void *target, const void *source, size_t size)
{
  if(target == NULL | source == NULL | size <= 0) return NULL;
  while(--size) *(uint8_t *)++target = *(uint8_t *)++source;
  return target;
}

void EXOSAPI *memset(void *target, const uint8_t data, size_t size)
{
  if(target == NULL | size <= 0) return NULL;
  while(--size) *(uint8_t *)++target = data;
  return target;
}
