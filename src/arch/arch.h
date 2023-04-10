#pragma once

#include <types>
#include <bootloader.h>

#ifdef __cplusplus  //  cpp函数
void archInit(BootConfig *conf);
#endif

#ifdef __cplusplus
extern "C" {    //  c函数
#endif

void platformInit(BootConfig *conf);

#ifdef __cplusplus
}
#endif

#if TARGET_ARCH == x86_64
#include <arch/x86_64/platform/platform.h>
#else
#error 无效的目标架构
#endif