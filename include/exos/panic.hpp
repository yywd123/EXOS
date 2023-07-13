#pragma once

#include <platform.hpp>

void
__panic(const char *msg, const char *sourceFileName, const char *functionName, uint32_t line, RegisterFrame *regFrame);

#define panic(msg)                                         \
	__panic(msg, __FILE__, __FUNCTION__, __LINE__, nullptr); \
	__builtin_unreachable()

#define panicWithReg(msg, reg)                         \
	__panic(msg, __FILE__, __FUNCTION__, __LINE__, reg); \
	__builtin_unreachable()
