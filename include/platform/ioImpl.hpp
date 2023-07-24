#ifndef __PLATFORM_H__
#error 请勿直接include这个头文件 请使用platform.h来代替
#endif

#pragma once

__NAMESPACE_DECL(Platform::IO)

static inline void
outb(uint16_t port, uint8_t value) {
	ASM("outb %0, %%dx" ::"a"(value), "d"(port));
}

static inline void
outw(uint16_t port, uint16_t value) {
	ASM("outw %0, %%dx" ::"a"(value), "d"(port));
}

static inline void
outl(uint16_t port, uint32_t value) {
	ASM("outl %0, %%dx" ::"a"(value), "d"(port));
}

static inline uint8_t
inb(uint16_t port) {
	uint8_t result = 0;
	ASM("inb %%dx, %0"
			: "=a"(result)
			: "d"(port));
	return result;
}

static inline uint16_t
inw(uint16_t port) {
	uint16_t result = 0;
	ASM("inw %%dx, %0"
			: "=a"(result)
			: "d"(port));
	return result;
}

static inline uint32_t
inl(uint16_t port) {
	uint32_t result = 0;
	ASM("inl %%dx, %0"
			: "=a"(result)
			: "d"(port));
	return result;
}

__NAMESPACE_END