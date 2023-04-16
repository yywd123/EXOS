#include <utils>

void *memcpy(void *dest, const void *src, unsigned long size) {
  if (dest == src) return dest;
  ASM("cld\n\trep movsb"::"rdi"(dest), "rsi"(src), "c"(size));
  return dest;
}

int memcmp(const void *buf1, const void *buf2, unsigned long size) {
  size_t i = 0;
  uint8_t *p1 = (uint8_t *)buf1;
  uint8_t *p2 = (uint8_t *)buf2;
  while(*p1++ == *p2++) ++i;

  if(i != size) return -1;

  return 0;
}

