#pragma once

#include <lib/guid.h>

typedef struct {
  GUID guid;
  void *table;
} EfiConfTableEntry;
