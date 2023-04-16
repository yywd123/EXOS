#pragma once
#include <arch/arch.h>

typedef enum {
  Undefinded,
  SoftwareException,
  HardwareException,
  UnrecoverableException
} ExceptionType;

#ifdef __cplusplus
extern "C" {
#endif
void raiseHardwareException(ExceptionType sourceType, IntrFrame *exceptionFrame);
#ifdef __cplusplus
}
#endif