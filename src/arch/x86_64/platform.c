#include <arch/arch.h>
#include <utils>

#define INTENTRY(n) extern void interruptEntry##n();
INTENTRY(00) 	//	#DE	除以0
INTENTRY(01) 	//	#DB	调试
INTENTRY(03) 	//	#BP	断点
INTENTRY(04) 	//	#OF 溢出
INTENTRY(05) 	//	#BR 数组索引超限(不知道有什么用
INTENTRY(06) 	//	#UD	未定义指令
INTENTRY(07) 	//	#NM	设备未就绪
INTENTRY(08) 	//	#DF	双重错误
INTENTRY(09) 	// 	--- 协处理器段溢出(不知道有什么用
INTENTRY(0a) 	//	#TS	无效的TSS
INTENTRY(0b) 	//	#NP	段不存在
INTENTRY(0c) 	//	#SS 堆栈段故障
INTENTRY(0d) 	//	#GP 一般保护故障
INTENTRY(0e) 	//	#PF	缺页
INTENTRY(10) 	//	#MF	x87浮点数异常
INTENTRY(11) 	//	#AC 内存数据引用未对齐(仅在Ring3触发
INTENTRY(12) 	//	#MC	处理器内部错误
INTENTRY(13) 	//	#XM/#XF	SIMD浮点数异常
INTENTRY(14) 	//	#VE	虚拟化异常
INTENTRY(15) 	//	#CP 控制保护异常(不知道有什么用
INTENTRY(1c) 	//	#HV	虚拟机注入的异常
INTENTRY(1d) 	//	#VC	VMM通信失败
INTENTRY(1e) 	//	#SX	安全异常
#undef INTENTRY

void registerInterruptHandlers() {
#define registerExceptionHandler(index, ist) registerInterrupt(0x##index, (uint64_t)interruptEntry##index , (1 << 3), ist, trap)
  registerExceptionHandler(00, 0);
	registerExceptionHandler(01, 0);
	registerExceptionHandler(03, 0);
	registerExceptionHandler(04, 0);
	registerExceptionHandler(05, 0);
	registerExceptionHandler(06, 0);
	registerExceptionHandler(07, 0);
	registerExceptionHandler(08, 0);
	registerExceptionHandler(09, 0);
	registerExceptionHandler(0a, 0);
	registerExceptionHandler(0b, 0);
	registerExceptionHandler(0c, 0);
	registerExceptionHandler(0d, 0);
	registerExceptionHandler(0e, 0);
	registerExceptionHandler(10, 0);
	registerExceptionHandler(11, 0);
	registerExceptionHandler(12, 0);
	registerExceptionHandler(13, 0);
	registerExceptionHandler(14, 0);
	registerExceptionHandler(15, 0);
	registerExceptionHandler(1c, 0);
	registerExceptionHandler(1d, 0);
	registerExceptionHandler(1e, 0);
  #undef registerExceptionHandler
}

void platformInit(BootConfig *conf) {
  makeGDTEntry(1, 0, SEGFLAG_EXCUTEABLE);
  makeGDTEntry(2, 0, SEGFLAG_RW);

  registerInterruptHandlers();

  loadDT();
}