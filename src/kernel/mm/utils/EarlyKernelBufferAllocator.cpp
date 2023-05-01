#include <mm/EarlyKernelBufferAllocator.hpp>

static uint8_t earlyKernelBuffer[0x10000];  //64kb
uint8_t* current = earlyKernelBuffer;
uint8_t* end = earlyKernelBuffer + sizeof(earlyKernelBuffer);

using namespace EXOS::Memory;

void *EarlyKernelBufferAllocator::allocate(size_t size) {
  if (current + size > end) {
    return nullptr;
  }
  void *p = current;
  current += size;
  return p;
}

void EarlyKernelBufferAllocator::free(void *) { }  //  使用这个分配器的内存肯定都不可能free