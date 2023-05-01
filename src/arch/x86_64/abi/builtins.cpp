#include <types.hpp>

extern "C" void *malloc(size_t size) {
  return (void*)new uint8_t[size];
}