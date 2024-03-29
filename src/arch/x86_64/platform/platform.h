#if TARGET_ARCH != x86_64
#error 请勿引用不兼容平台的头文件
#endif

#pragma once

#include <types.hpp>
#include "include/io.h"
#include "include/pic.h"

#define SEGFLAG_RW BIT(1)
#define SEGFLAG_EXCUTEABLE BIT(3)

#define __cpuid(__leaf, __eax, __ebx, __ecx, __edx) \
    asm volatile("xchgq %%rbx,%q1\n" \
        "cpuid\n" \
        "xchgq %%rbx,%q1" \
        : "=a"(__eax), "=r" (__ebx), "=c"(__ecx), "=d"(__edx) \
        : "0"(__leaf))

#define __cpuid_count(__leaf, __count, __eax, __ebx, __ecx, __edx) \
    asm volatile("xchgq %%rbx,%q1\n" \
        "cpuid\n" \
        "xchgq %%rbx,%q1" \
        : "=a"(__eax), "=r" (__ebx), "=c"(__ecx), "=d"(__edx) \
        : "0"(__leaf), "2"(__count))

#define INTENTRY(n) extern void interruptEntry##n();
INTENTRY(00) 	//	#DE	除以0
INTENTRY(01) 	//	#DB	调试
INTENTRY(03) 	//	#BP	断点
INTENTRY(04) 	//	#OF 溢出
INTENTRY(05) 	//	#BR 数组索引超限(不知道有什么用
INTENTRY(06) 	//	#UD	未定义指令
INTENTRY(07) 	//	#NM	设备未就绪
INTENTRY(08) 	//	#DF	双重错误
INTENTRY(0a) 	//	#TS	无效的TSS
INTENTRY(0b) 	//	#NP	段不存在
INTENTRY(0c) 	//	#SS 堆栈段故障
INTENTRY(0d) 	//	#GP 一般保护故障
INTENTRY(0e) 	//	#PF	缺页
INTENTRY(10) 	//	#MF	x87浮点数异常
INTENTRY(11) 	//	#AC 内存数据引用未对齐(仅在Ring3触发
INTENTRY(12) 	//	#MC	处理器内部错误
INTENTRY(13) 	//	#XM	SIMD浮点数异常
INTENTRY(14)
INTENTRY(15)
INTENTRY(16)
INTENTRY(17)
INTENTRY(18)
INTENTRY(19)
INTENTRY(1a)
INTENTRY(1b)
INTENTRY(1c)
INTENTRY(1d)
INTENTRY(1e)
INTENTRY(1f)
INTENTRY(20)
INTENTRY(21)
INTENTRY(22)
INTENTRY(23)
INTENTRY(24)
INTENTRY(25)
INTENTRY(26)
INTENTRY(27)
INTENTRY(28)
INTENTRY(29)
INTENTRY(2a)
INTENTRY(2b)
INTENTRY(2c)
INTENTRY(2d)
INTENTRY(2e)
INTENTRY(2f)
INTENTRY(30)
INTENTRY(31)
INTENTRY(32)
INTENTRY(33)
INTENTRY(34)
INTENTRY(35)
INTENTRY(36)
INTENTRY(37)
INTENTRY(38)
INTENTRY(39)
INTENTRY(3a)
INTENTRY(3b)
INTENTRY(3c)
INTENTRY(3d)
INTENTRY(3e)
INTENTRY(3f)
INTENTRY(40)
INTENTRY(41)
INTENTRY(42)
INTENTRY(43)
INTENTRY(44)
INTENTRY(45)
INTENTRY(46)
INTENTRY(47)
INTENTRY(48)
INTENTRY(49)
INTENTRY(4a)
INTENTRY(4b)
INTENTRY(4c)
INTENTRY(4d)
INTENTRY(4e)
INTENTRY(4f)
INTENTRY(50)
INTENTRY(51)
INTENTRY(52)
INTENTRY(53)
INTENTRY(54)
INTENTRY(55)
INTENTRY(56)
INTENTRY(57)
INTENTRY(58)
INTENTRY(59)
INTENTRY(5a)
INTENTRY(5b)
INTENTRY(5c)
INTENTRY(5d)
INTENTRY(5e)
INTENTRY(5f)
INTENTRY(60)
INTENTRY(61)
INTENTRY(62)
INTENTRY(63)
INTENTRY(64)
INTENTRY(65)
INTENTRY(66)
INTENTRY(67)
INTENTRY(68)
INTENTRY(69)
INTENTRY(6a)
INTENTRY(6b)
INTENTRY(6c)
INTENTRY(6d)
INTENTRY(6e)
INTENTRY(6f)
INTENTRY(70)
INTENTRY(71)
INTENTRY(72)
INTENTRY(73)
INTENTRY(74)
INTENTRY(75)
INTENTRY(76)
INTENTRY(77)
INTENTRY(78)
INTENTRY(79)
INTENTRY(7a)
INTENTRY(7b)
INTENTRY(7c)
INTENTRY(7d)
INTENTRY(7e)
INTENTRY(7f)
INTENTRY(80)
INTENTRY(81)
INTENTRY(82)
INTENTRY(83)
INTENTRY(84)
INTENTRY(85)
INTENTRY(86)
INTENTRY(87)
INTENTRY(88)
INTENTRY(89)
INTENTRY(8a)
INTENTRY(8b)
INTENTRY(8c)
INTENTRY(8d)
INTENTRY(8e)
INTENTRY(8f)
INTENTRY(90)
INTENTRY(91)
INTENTRY(92)
INTENTRY(93)
INTENTRY(94)
INTENTRY(95)
INTENTRY(96)
INTENTRY(97)
INTENTRY(98)
INTENTRY(99)
INTENTRY(9a)
INTENTRY(9b)
INTENTRY(9c)
INTENTRY(9d)
INTENTRY(9e)
INTENTRY(9f)
INTENTRY(a0)
INTENTRY(a1)
INTENTRY(a2)
INTENTRY(a3)
INTENTRY(a4)
INTENTRY(a5)
INTENTRY(a6)
INTENTRY(a7)
INTENTRY(a8)
INTENTRY(a9)
INTENTRY(aa)
INTENTRY(ab)
INTENTRY(ac)
INTENTRY(ad)
INTENTRY(ae)
INTENTRY(af)
INTENTRY(b0)
INTENTRY(b1)
INTENTRY(b2)
INTENTRY(b3)
INTENTRY(b4)
INTENTRY(b5)
INTENTRY(b6)
INTENTRY(b7)
INTENTRY(b8)
INTENTRY(b9)
INTENTRY(ba)
INTENTRY(bb)
INTENTRY(bc)
INTENTRY(bd)
INTENTRY(be)
INTENTRY(bf)
INTENTRY(c0)
INTENTRY(c1)
INTENTRY(c2)
INTENTRY(c3)
INTENTRY(c4)
INTENTRY(c5)
INTENTRY(c6)
INTENTRY(c7)
INTENTRY(c8)
INTENTRY(c9)
INTENTRY(ca)
INTENTRY(cb)
INTENTRY(cc)
INTENTRY(cd)
INTENTRY(ce)
INTENTRY(cf)
INTENTRY(d0)
INTENTRY(d1)
INTENTRY(d2)
INTENTRY(d3)
INTENTRY(d4)
INTENTRY(d5)
INTENTRY(d6)
INTENTRY(d7)
INTENTRY(d8)
INTENTRY(d9)
INTENTRY(da)
INTENTRY(db)
INTENTRY(dc)
INTENTRY(dd)
INTENTRY(de)
INTENTRY(df)
INTENTRY(e0)
INTENTRY(e1)
INTENTRY(e2)
INTENTRY(e3)
INTENTRY(e4)
INTENTRY(e5)
INTENTRY(e6)
INTENTRY(e7)
INTENTRY(e8)
INTENTRY(e9)
INTENTRY(ea)
INTENTRY(eb)
INTENTRY(ec)
INTENTRY(ed)
INTENTRY(ee)
INTENTRY(ef)
INTENTRY(f0)
INTENTRY(f1)
INTENTRY(f2)
INTENTRY(f3)
INTENTRY(f4)
INTENTRY(f5)
INTENTRY(f6)
INTENTRY(f7)
INTENTRY(f8)
INTENTRY(f9)
INTENTRY(fa)
INTENTRY(fb)
INTENTRY(fc)
INTENTRY(fd)
INTENTRY(fe)
INTENTRY(ff)
#undef INTENTRY

typedef enum {
  interrupt = 0x8e,
  trap = 0x8f
} IntrType;

typedef struct {
  uint64_t rax;
  uint64_t rbx;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rbp;
  uint64_t r8;
  uint64_t r9;
  uint64_t r10;
  uint64_t r11;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
} __attribute__((packed)) RegisterFrame;

typedef struct {
  RegisterFrame regs;
  uint64_t irqIndex;
  uint64_t errCode;
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
} __attribute__((packed)) IntrFrame;

void makeGDTEntry(uint8_t index, unsigned privilege, unsigned flag);
void makeGDTSSEntry(uint8_t index, void *base, uint32_t limit, uint8_t flag);
void registerInterrupt(uint8_t index, uintptr_t entryPoint, uint16_t selector, uint8_t ist, IntrType type);
void loadDT();

/**
 * @brief 判断中断是否为异常并尝试处理被引起异常中断
 * 
 * @param frame 中断寄存器帧 
 * 
 * @return 0表示中断为异常且已处理完成
 * @return 1表示中断不是异常中断
 */
bool platformTryHandleException(IntrFrame *frame);
