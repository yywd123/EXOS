#pragma once

#include <utils/timeunit.hpp>

__NAMESPACE_DECL(Drivers::HPET)

void __INIT
initialize();

uint64_t
nanoTime();

uint64_t
rawTime();

uint64_t
getPeriod();

void
sleep(uint64_t nano);

static inline void
sleep(uint64_t time, Utils::TimeUnit::TimeUnit unit) {
	sleep(Utils::TimeUnit::convert(unit, time, Utils::TimeUnit::NANOSECONDS));
}

__NAMESPACE_END