#pragma once

__NAMESPACE_DECL(Drivers::Hpet)

void
initialize();

uint64_t
nanoTime();

void
sleep(uint64_t nano);

__NAMESPACE_END