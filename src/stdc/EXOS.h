#include <types.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

// defines
#define EXOSAPI __attribute__((__fastcall__))
#define EXOSMOD __attribute__((__stdcall__))
#define asmlinkage __attribute__((__cdecl__))
#define HWIO __attribute__((__fastcall__))
