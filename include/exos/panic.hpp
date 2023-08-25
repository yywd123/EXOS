#pragma once

#include <platform/platform.hpp>

void
__panic(const char *msg, const char *sourceFileName, const char *functionName, uint32_t line, InterruptFrame *regFrame);

#define panic(msg)                                           \
	do {                                                       \
		__panic(msg, __FILE__, __FUNCTION__, __LINE__, nullptr); \
		while(1)                                                 \
			;                                                      \
	} while(0)

#define panicWithReg(msg, reg)                           \
	do {                                                   \
		__panic(msg, __FILE__, __FUNCTION__, __LINE__, reg); \
		while(1)                                             \
			;                                                  \
	} while(0)

#define __UNREACHABLE __builtin_unreachable()