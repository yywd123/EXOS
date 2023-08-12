#include <exos/logger.hpp>

USE(EXOS::Utils);

//  这是为了便于移植c库而实现的一些函数

extern "C" {

void *
malloc(size_t size) {
	return (void *)new uint8_t[size];
}

void
free(void *p) {
	delete(uint8_t *)p;
}

int
puts(const char *str) {
	Logger::print(str);
	return length(str);
}
}