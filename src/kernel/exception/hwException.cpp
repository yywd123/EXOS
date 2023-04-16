#include <hwException.h>
#include <lib/SerialOutputStream>
#include <lib/PrintWriter>

using namespace EXOS;
using namespace EXOS::Driver;

extern "C" void raiseHardwareException(ExceptionType sourceType, IntrFrame *exceptionFrame) {
  Utils::SerialOutputStream debugStream(Serial::COM1);
  Utils::PrintWriter logger(&debugStream);

  if (sourceType == Undefinded) {
    logger.println("!!! 无效的异常类型 !!!");
    while (1) ASM("hlt");
  } else if (sourceType == SoftwareException) {
    logger.println("可恢复的中断");
    return;
  } else {
    logger.println("不可恢复的中断");
    while (1) ASM("hlt");
  }
}