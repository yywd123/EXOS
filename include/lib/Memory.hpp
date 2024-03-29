#pragma once

#include <utils.hpp>

namespace EXOS::Utils::Memory {
  static inline void memset(void *dest, uint8_t value, size_t n) {
    ASM("cld\n\trep stosb"::"rdi"(dest), "a"(value), "c"(n));
  }

  void memcpy(void *dest, void *src, size_t n) {
    if (!src) return;
    ASM("cld\n\trep movsb"::"rdi"(dest), "rsi"(src), "c"(n));
  }

}