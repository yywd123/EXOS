/*  Kernel Funtion Lib for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-24
*/

#include <OSBase.h>

extern VideoInfo Vinfo;

/********************/
#include <io/io.h>

#define iret __asm__ __volatile__("iret")

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define COM5 0x5F8
#define COM6 0x4F8
#define COM7 0x5E8
#define COM8 0x4E8

/********************/
#include <debug/debug.h>

#define LOG_EMERG 0xff
#define LOG_ALERT 0xee
#define LOG_CRIT 0xdd
#define LOG_ERROR 0xcc
#define LOG_WARN 0xbb
#define LOG_INFO 0xaa
#define LOG_DEBUG 0x99

/********************/
#include <expection.h>
