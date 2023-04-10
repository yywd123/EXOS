#include <utils>
#include <arch/arch.h>
#include <lib/SerialOutputStream>
#include <lib/PrintWriter>
#include <driver/Serial>

using namespace EXOS;
using namespace EXOS::Driver;

void archInit(BootConfig *conf) {
  Utils::SerialOutputStream earlyDebugStream(Driver::Serial::COM1);
  Utils::PrintWriter writer(&earlyDebugStream);
  platformInit(conf);
  writer.println("platform init ok");
  ASM("sti\n\tint3");
  writer.println("returned from intr handle");
}