#pragma once

__NAMESPACE_DECL(Memory::KMemory)

void
initialize();

void *
alloc(size_t size);

void *
alloc4KPage(uint32_t count);

void *
alloc2MPage(uint32_t count);

void
free(void *p);

__NAMESPACE_END