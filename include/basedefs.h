#pragma once

#include <stdint.h>
#include <wchar.h>

#include <attribute.h>

typedef void *Handle;
typedef uint64_t Status;

#define __iter(n) for(uint64_t i = 0; i < n; ++i)

#define BIT(n) (1ull << n)

#define ASM __asm__ __volatile__

#define getAddressFromSymbol(out, symbol) ASM("lea " symbol "(%%rip), %0":"=r"(out))

#define checkFlag(var, flag) ((var & flag) == 0 ? 0 : 1)
#ifdef __cplusplus
#include <basedefs.hpp>
#endif