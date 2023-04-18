#include <lib/Logger>

namespace EXOS::Utils::Logger {
  const char *SectionName = "kernel";
  Utils::OutputStream loggerStream = nullptr;

  void beginSection(const char *name) {
    SectionName = name;
  }

  void endSection() {
    SectionName = "kernel";
  }
}