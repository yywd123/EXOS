#pragma once

#include <utils.hpp>

namespace EXOS::Utils::String {
  static inline size_t length(const char* str) {
    size_t i;
    for (i = 0; str[i] != 0; ++i);
    return i;
  }
}