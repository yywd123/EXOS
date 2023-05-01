#pragma once

#include <mm/MemoryAllocator.hpp>

namespace EXOS::Memory {
  class EarlyKernelBufferAllocator : public _MemoryAllocator {
  public:
    void *allocate(size_t size);
    void free(void *p);

    void *operator new(size_t, void *buf) {
      return buf;
    }
  };
}