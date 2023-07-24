#pragma once
#define __PLATFORM_H__

__NAMESPACE_DECL(Platform)

void __INIT
initialize();

__NAMESPACE_END

#include "ioImpl.hpp"
#include "cpu.hpp"
#include "smp.hpp"
#include "interrupt.hpp"
#include "apic.hpp"