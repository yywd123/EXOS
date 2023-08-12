#include <efi/efi.hpp>
#include <mm/mm.hpp>
#include <exos/logger.hpp>

USE(EXOS::Memory);
USE(EXOS::Utils);

static size_t allocatedSize = 0;
static uint16_t allocateCount = 0;

void *
operator new(size_t size) {
	Logger::log(Logger::DEBUG, "memory alloc: @bytes", (int64_t)size);
	++allocateCount;
	allocatedSize += size;
	return EFI::allocatePool(size);
}

void *
operator new[](size_t size) {
	Logger::log(Logger::DEBUG, "memory alloc: @bytes", (int64_t)size);
	++allocateCount;
	allocatedSize += size;
	return EFI::allocatePool(size);
}

void
operator delete(void *p) {
	EFI::freePool(p);
}

void
operator delete(void *p, size_t) {
	EFI::freePool(p);
}

void
dumpAllocationInfo() {
	Logger::log(Logger::DEBUG, "memory: @bytes allocated(@times)", (int64_t)allocatedSize, (int16_t)allocateCount);
}

namespace std {
//	你这辈子就是被throw害了
void
__throw_length_error(char const *) {
	__builtin_unreachable();
}

void
__throw_bad_array_new_length() {
	__builtin_unreachable();
}

void
__throw_bad_alloc() {
	__builtin_unreachable();
}

void
__throw_out_of_range_fmt(char const *, ...) {
	__builtin_unreachable();
}

}	 // namespace std
