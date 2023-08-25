#ifndef __PLATFORM_H__
#error 请勿直接include这个头文件 请使用platform.h来代替
#endif

#pragma once

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
} __packed RegisterFrame;

typedef struct {
	RegisterFrame regs;
	uint64_t irqIndex;
	uint64_t errCode;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __packed InterruptFrame;

typedef struct {
	uint32_t unused0;
	uint8_t unused1;
	unsigned flag : 4;
	unsigned descType : 1;
	unsigned privilege : 2;
	bool isVaild : 1;
	unsigned unused2 : 4;
	unsigned reserved : 1;
	bool isLongModeCode : 1;
	unsigned unused3 : 2;
	uint8_t unused4;
} __packed SegmentDescriptor;

typedef struct {
	uint16_t limit0;
	uint16_t base0;
	uint8_t base1;
	unsigned flag : 4;
	unsigned descType : 1;
	unsigned privilege : 2;
	bool isVaild : 1;
	unsigned limit1 : 4;
	unsigned usused : 4;
	uint8_t base2;
	uint32_t base3;
	uint32_t reserved;
} __packed SystemSegmentDescriptor;

typedef struct {
	uint16_t offset0;
	uint16_t selector;
	uint8_t ist;
	uint8_t attributes;
	uint16_t offset1;
	uint32_t offset2;
	uint32_t reserved;

	void
	set(uintptr_t entryPoint, uint16_t selector, uint8_t ist, uint8_t type) {
		this->offset0 = (entryPoint & 0xffff);
		this->selector = selector;
		this->ist = (ist & 7);
		this->attributes = (type & 0xff);
		this->offset1 = ((entryPoint >> 16) & 0xffff);
		this->offset2 = ((entryPoint >> 32) & 0xffffffff);
		this->reserved = 0;
	}
} __packed InterruptDescriptor;

#define TSS_Available 0x9
#define TSS_Busy 0xb

typedef struct {
	uint32_t reserved0;
	uintptr_t rsp[3];
	uint64_t reserved1;
	uintptr_t ist[7];
	uint64_t reserved2;
	uint16_t reserved3;
	uint16_t iopb;
} __attribute__((packed)) TaskStateSegment;

#define __cpuid(__func, __eax, __ebx, __ecx, __edx)        \
	ASM("xchgq %%rbx,%q1\n"                                  \
			"cpuid\n"                                            \
			"xchgq %%rbx,%q1"                                    \
			: "=a"(__eax), "=r"(__ebx), "=c"(__ecx), "=d"(__edx) \
			: "0"(__func))

__NAMESPACE_DECL(Platform::Processor)

typedef struct {
	uint8_t coreApicId;
} Core;

void __INIT
initialize();

uint8_t
getCurrentCoreApicID();

__NAMESPACE_END