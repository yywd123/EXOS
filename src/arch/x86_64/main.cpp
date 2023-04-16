#include <utils>
#include <arch/arch.h>
#include <lib/SimpleOutputStream>
#include <lib/PrintWriter>
#include <driver/Serial>

using namespace EXOS;
using namespace EXOS::Driver;

extern "C" void platformInit(BootConfig *conf);

void archInit(BootConfig *conf) {
  Utils::SimpleOutputStream earlyDebugStream([](uint8_t byte) {
    Serial::write(Serial::COM1, byte);
  });
  Utils::PrintWriter logger(&earlyDebugStream);
  platformInit(conf);

  logger.println("memory allocation test");
  iter(30) {
    const char* buf = new char[7];
    __builtin_memcpy((void*)buf, "114514", 7);
    logger.println(buf);
  }
}