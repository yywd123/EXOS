#pragma once

#include <types.hpp>
#include <bootloader.h>
#include <archinfo.h>

#ifndef TARGET_ARCH
#error 未指定目标架构
#endif

#ifdef __cplusplus
extern "C" {
#define __C_LINKAGE__
#endif

#if TARGET_ARCH == x86_64
#include <arch/x86_64/platform/platform.h>
#else
#error 无效的目标架构
#endif

#ifdef __cplusplus
#undef __C_LINKAGE__
}
#endif