#include <utils>
#include <arch/arch.h>
#include <lib/SerialOutputStream>
#include <lib/PrintWriter>

using namespace EXOS;

extern "C" IntrFrame *handleInterrupt(IntrFrame *frame) {
  Utils::SerialOutputStream os(Driver::Serial::COM1);
  Utils::PrintWriter logger(&os);
  logger.println("intr handled");
  iter(frame->irqIndex) logger.print(".");
  logger.println("");
  return frame;
}