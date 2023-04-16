#include <arch/arch.h>
#include <hwException.h>

bool platformTryHandleException(IntrFrame *frame) {
  if (frame->irqIndex >= 0x20) return 1;

  ExceptionType type;

  switch (frame->irqIndex) {
    case 0x01:
    case 0x03:
    case 0x04:
      type = SoftwareException;
      break;
    case 0x00:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x0a:
    case 0x0b:
    case 0x0c:
    case 0x0d:
    case 0x0e:
    case 0x10:
    case 0x11:
    case 0x13:
      type = HardwareException;
      break;
    case 0x08:
    case 0x12:
      type = UnrecoverableException;
      break;
    default:
      type = Undefinded;
  }

  raiseHardwareException(type, frame);

  return 0; 
}