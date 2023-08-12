#pragma once

#include <utils/guid.hpp>
#include <exos/fbcon.hpp>
#include <exos/serial.hpp>
#include <exos/cmos.hpp>
#include <platform/platform.hpp>
#include <exos/hpet.hpp>

__NAMESPACE_DECL(Utils::Logger)

static inline void
print(char c) {
	Drivers::FbConsole::print(c);
	Drivers::Serial::write(Drivers::Serial::COM1, c);
}

static inline void
print(const char *s) {
	_iter(length(s)) print(s[i]);
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
		_iter(n) print('0');
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
print(int8_t i) {
	printInt(i);
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
	_iter(8) print(guid.field4[i]);
}

static inline void
print(Drivers::CMOS::Time time) {
	print((int16_t)time.year);
	print('-');
	print((int8_t)time.month);
	print('-');
	print((int8_t)time.day);
	print(' ');
	if(time.hours < 10) print('0');
	print((int8_t)time.hours);
	print(':');
	if(time.minutes < 10) print('0');
	print((int8_t)time.minutes);
	print(':');
	if(time.seconds < 10) print('0');
	print((int8_t)time.seconds);
}

template<typename T>
static inline void
print(T *p) {
	if(p == nullptr) {
		print("ptr[nullptr]");
		return;
	}
	if(p == (T *)BAD_PTR) {
		print("ptr[unilitialized]");
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
printf(const char *fmt) {
	while(*fmt) {
		if(*fmt == '@' && *(++fmt) != '@') {
			print("???");
		}
		print(*fmt++);
	}
}

/**
 * @bug 有时候我们想要用printf打印只打印一个值 传进来的fmt直接就是"@"
 * 			这样会无法打印
 * 			解决方法:	随便在fmt字符串里面加点什么就正常了 千万不要只有@
 */
template<typename T, typename... Args>
static inline void
printf(const char *fmt, const T value, const Args... args) {
	while(*fmt) {
		if(*fmt == '@' && *(++fmt) != '@') {
			print(value);
			printf(fmt, args...);
			return;
		}
		print(*fmt++);
	}
}

typedef enum {
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FATAL
} LogLevel;

template<typename... Args>
static inline void
log(LogLevel level, const char *msg, const Args... args) {
#ifdef RELEASE
	if(level == DEBUG) return;
#endif
	Drivers::FbConsole::setColor(false, 0xb8b8b8);
	print(Drivers::Hpet::rawTime());
	print(' ');
	print(Platform::MultiProcessor::getCurrentCoreApicID());
	print(" [");
	switch(level) {
	default:
	case DEBUG:
		Drivers::FbConsole::setColor(false, 0x018070);
		print("DEBUG");
		break;
	case INFO:
		Drivers::FbConsole::setColor(false, 0x2ecc40);
		print("INFO");
		break;
	case WARN:
		Drivers::FbConsole::setColor(false, 0xff851b);
		print("WARN");
		break;
	case ERROR:
		Drivers::FbConsole::setColor(false, 0xff4136);
		print("ERROR");
		break;
	case FATAL:
		Drivers::FbConsole::setColor(false, 0xaa0000);
		print("FATAL");
		break;
	}
	Drivers::FbConsole::setColor(false, 0xb8b8b8);
	print("] ");
	printf(msg, args...);
	print('\n');
}

__NAMESPACE_END