#include <hwException.h>
#include <lib/Logger.hpp>

using namespace EXOS::Utils;
using namespace EXOS::Driver;

extern "C" void raiseHardwareException(ExceptionType sourceType, IntrFrame *exceptionFrame) {
  if (sourceType == Undefinded) {
    Logger::log(Logger::DEBUG, "undefinded intr type");
    while (1) ASM("hlt");
  } else if (sourceType == SoftwareException) {
    Logger::log(Logger::DEBUG, "recoverable intr type");
    return;
  } else {
    Logger::log(Logger::DEBUG, "unrecoverable intr type");
    while (1) ASM("hlt");
  }
}