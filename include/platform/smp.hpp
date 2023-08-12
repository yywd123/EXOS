#ifndef __PLATFORM_H__
#error 请勿直接include这个头文件 请使用platform.h来代替
#endif

#pragma once

__NAMESPACE_DECL(Platform::MultiProcessor)

typedef struct {
	uint8_t coreApicId;

	InterruptDescriptor idt[256] = {0};
} Core;

void __INIT
initialize();

uint8_t
getCurrentCoreApicID();

__NAMESPACE_END