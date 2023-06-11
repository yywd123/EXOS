#pragma once

__NAMESPACE_DECL(Memory)

class AbstractMemoryAllocator {
public:
  virtual void 
  *alloc(size_t size) = 0;

  virtual void 
  free(void *p) = 0;
};

using MemoryAllocator = Ptr<AbstractMemoryAllocator>;

__NAMESPACE_END
