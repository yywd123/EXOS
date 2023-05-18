#pragma once

#include <types.hpp>
#include <bootloader.h>

#ifdef __cplusplus
extern "C" {
#define __C_LINKAGE__
#endif

#if defined(__x86_64__)
#include <arch/x86_64/platform/platform.h>
#elif defined(__aarch64__)
#include <arch/aarch64/platform/platform.h>
#else
#error 无效的目标架构
#endif

#ifdef __cplusplus
#undef __C_LINKAGE__
}
#endif