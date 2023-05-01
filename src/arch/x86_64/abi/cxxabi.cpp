#include <utils.hpp>
#include <lib/Logger.hpp>
#include <mm/EarlyKernelBufferAllocator.hpp>

using namespace EXOS::Memory;
using namespace EXOS::Utils;

MemoryAllocator allocator;

void cxxabiEarlyInit() {
  allocator = new(EarlyKernelBufferAllocator().allocate(sizeof(EarlyKernelBufferAllocator))) EarlyKernelBufferAllocator();
}

void *operator new(size_t size) {
  Logger::log(Logger::DEBUG, "allocate memory, size 0x@", size);
  return allocator->allocate(size);
}

void *operator new[](size_t size) {
  Logger::log(Logger::DEBUG, "allocate memory, size 0x@", size);
  return allocator->allocate(size);
}

void operator delete(void *p) {
  allocator->free(p);
}

void operator delete(void *p, size_t) {
  allocator->free(p);
}