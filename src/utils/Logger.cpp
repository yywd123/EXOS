#include <lib/Logger.hpp>

namespace EXOS::Utils::Logger {
  const char *sectionName = "kernel";
  const char *savedSectionNameStack[128];
  uint8_t stackIndex = 0;;
  Utils::OutputStream loggerStream = nullptr;

  void beginSection(const char *name) {
    sectionName = name;
  }

  void endSection() {
    sectionName = "kernel";
  }

  void saveAndBeginSection(const char *name) {
    if (stackIndex >= 0x80) return;
    savedSectionNameStack[stackIndex++] = sectionName;
    sectionName = name;
  }

  void restoreSection() {
    sectionName = savedSectionNameStack[--stackIndex];
  }
}