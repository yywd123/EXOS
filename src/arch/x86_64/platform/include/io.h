#pragma once

#include <utils.hpp>

#ifdef __cplusplus 
namespace EXOS::Driver::IO {
#endif
  static inline void outb(uint16_t port, uint8_t data) {
    ASM("outb %0, %%dx"::"a"(data), "d"(port));
  }

  static inline void outw(uint16_t port, uint16_t data) {
    ASM("outw %0, %%dx"::"a"(data), "d"(port));
  }

  static inline void outl(uint16_t port, uint32_t data) {
    ASM("outl %0, %%dx"::"a"(data), "d"(port));
  }

  static inline uint8_t inb(uint16_t port) {
    uint8_t result = 0;
    ASM("inb %%dx, %0"
        :"=a"(result)
        :"d"(port));
    return result;
  }

  static inline uint16_t inw(uint16_t port) {
    uint16_t result = 0;
    ASM("inw %%dx, %0"
        :"=a"(result)
        :"d"(port));
    return result;
  }

  static inline uint32_t ind(uint16_t port) {
    uint32_t result = 0;
    ASM("inl %%dx, %0"
        :"=a"(result)
        :"d"(port));
    return result;
  }
#ifdef __cplusplus 
}
#endif