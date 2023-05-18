#pragma once

#include <types.hpp>

extern "C" bool archSetTimerFrequency(uint64_t hz);
extern "C" uint64_t archGetTimerFrequency();
extern "C" void archTimerSleep(uint64_t microsecond);

namespace EXOS::Arch::Timer {
  static inline bool setTimerFrequency(uint64_t hz) {
    archSetTimerFrequency(hz);
  }

  static inline uint64_t getTimerFrequency() {

  }

  static inline void sleep(uint64_t microsecond) {
    archTimerSleep(microsecond);
  }
}