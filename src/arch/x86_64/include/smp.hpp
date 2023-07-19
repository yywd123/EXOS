#pragma once

#include "cpu.hpp"

__NAMESPACE_DECL(Platform::MultiProcessor)

typedef struct {
	uint8_t coreApicId;

	InterruptDescriptor *idt;
} Core;

void __INIT
initialize();

__NAMESPACE_END