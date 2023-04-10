#include <utils>
#include <bootloader.h>

extern void archInit(BootConfig *conf);

extern "C" attr(noreturn) void kernelEntry(BootConfig *conf) {
  archInit(conf);
  while (1);
  __builtin_unreachable();
}