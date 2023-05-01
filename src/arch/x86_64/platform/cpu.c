#include <arch/arch.h>

uint8_t platformGetCoreApicID() {
  uint32_t apicID = 0;
  uint32_t tmp;
  __cpuid(0x1, tmp, apicID, tmp, tmp);
  return (apicID >> 24);
}