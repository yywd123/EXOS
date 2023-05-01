#include <utils.hpp>
#include <arch/arch.h>
#include <lib/Logger.hpp>
#include <driver/Serial.hpp>

using namespace EXOS::Utils;
using namespace EXOS::Driver;

extern "C" void platformInit(BootConfig *conf);

void archInit(BootConfig *conf) {
  platformInit(conf);
}