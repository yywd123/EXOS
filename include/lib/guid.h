#pragma once

#include <types.hpp>

typedef struct {
  uint32_t field1;
  uint16_t field2;
  uint16_t field3;
  uint8_t field4[8];
} __attribute__((packed)) GUID;

#define __GUID(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) (GUID)a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11

static inline bool GuidCompare(GUID guid1, GUID guid2) {
  return __builtin_memcmp(&guid1, &guid2, sizeof(GUID)) == 0;
}
