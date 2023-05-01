#pragma once

#include <utils.hpp>

namespace EXOS::Memory {
  class _MemoryAllocator {
  public:
    virtual void *allocate(size_t size) = 0;
    virtual void free(void *p) = 0;
  };

  using MemoryAllocator = _MemoryAllocator*;
}