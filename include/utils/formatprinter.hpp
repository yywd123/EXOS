#pragma once

#include <utils/guid.hpp>
#include <exos/fbcon.hpp>
#include <exos/serial.hpp>

__NAMESPACE_DECL(Utils::FormatPrinter)

static inline void
print(char c) {
	Drivers::FbConsole::print(c);
	Drivers::Serial::write(Drivers::Serial::COM1, c == '\x1b' ? '\b' : c);
}

static inline void
print(const char *s) {
	__iter(length(s)) print(s[i]);
}

static inline void
printInt(int64_t i) {
	if(i < 0) {
		print('-');
		i = -i;
	}
	if(i > 9) {
		printInt(i / 10);
	}
	print('0' + i % 10);
}

static inline void
printUInt(uint64_t i, uint8_t n) {	//  统一打印十六进制
	if(i == 0) {
		__iter(n) print('0');
		return;
	}
	const char *digits = "0123456789abcdef";
	char buf[17] = {0};
	for(uint8_t j = 16; j != 0; --j) {
		buf[j - 1] = digits[i & 0xf];
		i >>= 4;
	}

	const char *p = &buf[16 - n];

	print(p);
}

static inline void
print(uint8_t byte) {
	printUInt(byte, 2);
}

static inline void
print(int16_t i) {
	printInt(i);
}

static inline void
print(uint16_t i) {
	printUInt(i, 4);
}

static inline void
print(int32_t i) {
	printInt(i);
}

static inline void
print(uint32_t i) {
	printUInt(i, 8);
}

static inline void
print(int64_t i) {
	printInt(i);
}

static inline void
print(uint64_t i) {
	printUInt(i, 16);
}

static inline void
print(index_t i) {
	print('#');
	printInt(i);
}

static inline void
print(bool b) {
	if(b)
		print("true");
	else
		print("false");
}

static inline void
print(GUID guid) {
	print(guid.field1);
	print('-');
	print(guid.field2);
	print('-');
	print(guid.field3);
	print('-');
	__iter(8) print(guid.field4[i]);
}

template<typename T>
static inline void
print(T *p) {
	if(p == nullptr) {
		print("ptr[nullptr]");
		return;
	}
	print("ptr[0x");
	printUInt((uint64_t)p, 16);
	print(']');
}

template<typename T>
static inline void
print(T) {
	print("[unknown type]");
}

static inline void
printf(const char *s) {
	while(*s) {
		if(*s == '@' && *(++s) != '@') {
			print("???");
		}
		print(*s++);
	}
}

template<typename T, typename... Args>
static inline void
printf(const char *s, const T value, const Args... args) {
	while(*s) {
		if(*s == '@' && *(++s) != '@') {
			print(value);
			printf(s, args...);
			return;
		}
		print(*s++);
	}
}

__NAMESPACE_END