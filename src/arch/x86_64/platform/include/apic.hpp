#pragma once

#include <types.hpp>

namespace EXOS::Driver::APIC {
  void writeLapic(uint32_t reg, uint32_t value);
  uint32_t readLapic(uint32_t reg);
  void writeIOapic(uint32_t reg, uint32_t value);
  uint32_t readIOapic(uint32_t reg);
}