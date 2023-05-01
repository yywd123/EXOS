#include <utils.hpp>
#include <arch/arch.h>
#include <lib/Logger.hpp>

#include <input/keyboard.h>

using namespace EXOS::Utils;
using namespace EXOS::Driver;

extern "C" IntrFrame *handleInterrupt(IntrFrame *frame) {
  // Logger::log(Logger::DEBUG, "intr handled, irqIndex is @", frame->irqIndex);
  if (platformTryHandleException(frame)) {
    //Logger::log(Logger::DEBUG, "this interrupt isn't a exception");
  }
  if (frame->irqIndex == 0x21) handleKeyboardInput();
  return frame;
}

