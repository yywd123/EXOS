#pragma once

__NAMESPACE_DECL(Memory)

void
initialize();

void *
__allocate(size_t size);

template<typename T = uint8_t>
T *
allocate(size_t size) {
	return (T *)__allocate(size * sizeof(T));
}

__NAMESPACE_END