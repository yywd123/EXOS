#pragma once

#include "cpu.hpp"

__NAMESPACE_DECL(Platform::MultiProcessor)

typedef struct {
  uint32_t coreApicId;

  InterruptDescriptor idt[256] = {0};
} SMPCore;

void __INIT
initBootstrapProcessor();

void __INIT
initApplicationProcessors();

__NAMESPACE_END