#pragma once

#include <stdint.h>
#include <wchar.h>

#include <attribute.h>

typedef void *Handle;
typedef uint64_t Status;

typedef long long index_t;

#define BAD_PTR 0xafafafafafafafaf

#define __iter(val, n) for(index_t val = 0; val < (index_t)(n); ++val)
#define _iter(n) __iter(i, n)
#define loop while(1)

#define BIT(n) (1ull << (n))

#define ASM __asm__ __volatile__

#define getAddressFromSymbol(out, symbol) ASM("lea " symbol "(%%rip), %0" \
																							: "=r"(out))

#define checkFlag(var, flag) ((var & flag) == 0 ? 0 : 1)

#define __divceil(a, b) (((a) + (b)-1) / (b))

#ifdef __cplusplus
#include <basedefs.hpp>
#endif