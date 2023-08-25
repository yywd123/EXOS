#pragma once
#define __PLATFORM_H__

__NAMESPACE_DECL(Platform)

void __INIT
initialize();

__NAMESPACE_END

#include "ioImpl.hpp"
#include "processor.hpp"
#include "interrupt.hpp"
#include "apic.hpp"