.macro INTENTRY n
.global interruptEntry\n
interruptEntry\n:
	pushq $0
	pushq $0x\n
	leaq _handleIntr, %rax
	jmp *%rax
.endm

.macro INTENTRY_ERR n
.global interruptEntry\n
interruptEntry\n:
	pushq $0x\n
	leaq _handleIntr, %rax
	jmp *%rax
.endm

INTENTRY 00 	//	#DE	除以0
INTENTRY 01 	//	#DB	调试
INTENTRY 03 	//	#BP	断点
INTENTRY 04 	//	#OF 溢出
INTENTRY 05 	//	#BR 数组索引超限 不知道有什么用
INTENTRY 06 	//	#UD	未定义指令
INTENTRY 07 	//	#NM	设备未就绪
INTENTRY_ERR 08 	//	#DF	双重错误
INTENTRY 09 	// 	--- 协处理器段溢出 不知道有什么用
INTENTRY_ERR 0a 	//	#TS	无效的TSS
INTENTRY_ERR 0b 	//	#NP	段不存在
INTENTRY_ERR 0c 	//	#SS 堆栈段故障
INTENTRY_ERR 0d 	//	#GP 一般保护故障
INTENTRY_ERR 0e 	//	#PF	缺页
INTENTRY 10 	//	#MF	x87浮点数异常
INTENTRY 11 	//	#AC 内存数据引用未对齐 仅在Ring3触发
INTENTRY 12 	//	#MC	处理器内部错误
INTENTRY 13 	//	#XM/#XF	SIMD浮点数异常
INTENTRY 14 	//	#VE	虚拟化异常
INTENTRY 15 	//	#CP 控制保护异常 不知道有什么用
INTENTRY 1c 	//	#HV	虚拟机注入的异常
INTENTRY 1d 	//	#VC	VMM通信失败
INTENTRY 1e 	//	#SX	安全异常

.extern handleInterrupt
_handleIntr:
	push %rax
	push %rbx
	push %rcx
	push %rdx
	push %rsi
	push %rdi
	push %rbp
	push %r8
	push %r9
	push %r10
	push %r11
	push %r12
	push %r13
	push %r14
	push %r15

	mov %rsp, %rdi
	leaq handleInterrupt, %rax
	call *%rax
	mov %rax, %rsp

	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %r11
	pop %r10
	pop %r9
	pop %r8
	pop %rbp
	pop %rdi
	pop %rsi
	pop %rdx
	pop %rcx
	pop %rbx
	pop %rax

	add $16, %rsp
	iretq
