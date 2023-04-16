#include <utils>
#include <mm/EarlyKernelBufferAllocator>

using namespace EXOS::Memory;

MemoryAllocator allocator;

void cxxabiEarlyInit() {
  allocator = new(EarlyKernelBufferAllocator().allocate(sizeof(EarlyKernelBufferAllocator))) EarlyKernelBufferAllocator();
}

void *operator new(size_t size) {
  return allocator->allocate(size);
}

void *operator new[](size_t size) {
  return allocator->allocate(size);
}

void operator delete(void *p) {
  allocator->free(p);
}

void operator delete(void *p, size_t) {
  allocator->free(p);
}