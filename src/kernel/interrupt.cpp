#include <utils>
#include <arch/arch.h>
#include <lib/SerialOutputStream>
#include <lib/PrintWriter>

using namespace EXOS;

extern "C" IntrFrame *handleInterrupt(IntrFrame *frame) {
  Utils::SerialOutputStream os(Driver::Serial::COM1);
  Utils::PrintWriter logger(&os);
  logger.println("中断触发");
  iter(frame->irqIndex) logger.print(".");
  logger.println("");
  if (platformTryHandleException(frame)) {
    logger.println("这是一个普通的中断");
  }
  return frame;
}