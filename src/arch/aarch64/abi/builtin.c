#include <utils.hpp>

void *memcpy(void *dest, const void *src, size_t size) {
  uint8_t *dest_ = (uint8_t*)dest;
  uint8_t *src_ = (uint8_t*)src;
  while (size--) {
    //ASM("pld [%%0, #128]"::"r"(src));
    *dest_++ = *src_++;
  }

  return dest;
}