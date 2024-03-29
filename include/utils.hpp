#pragma once

#include <types.hpp>

//  下面是一些常用的工具

#define BIT(x) (1ull << (x))
#define checkFlag(field, flag) (field & flag)

#define str(macro) __str(macro)
#define __str(macro) #macro
#define CRLF "\r\n"
#define CRLFstr(s) (s CRLF)
#define str_length(s) s, sizeof(s) - 1

#define ASM __asm__ __volatile__

#define attr(x, ...) __attribute__((x, ##__VA_ARGS__))

#define _BARRIER "mfence\n\t"
#define BARRIER ASM(_BARRIER);

#define iter(n) for (uint64_t i = 0; i < (n); ++i)
#define match(x, __code__) case x: __code__; break;

#ifdef __cplusplus
extern "C++" {
template<typename T, typename... O>
using Function = T(*)(O...);

template<typename T>
using Pointer = T*;
}
#endif
