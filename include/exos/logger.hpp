#pragma once

#include <utils/formatprinter.hpp>

__NAMESPACE_DECL(Utils::Logger)

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
	Drivers::FbConsole::setColor(false, 0xb8b8b8);
	FormatPrinter::print('[');
	switch(level) {
	default:
	case DEBUG:
		Drivers::FbConsole::setColor(false, 0x018070);
		FormatPrinter::print("DEBUG");
		break;
	case INFO:
		Drivers::FbConsole::setColor(false, 0x2ecc40);
		FormatPrinter::print("INFO");
		break;
	case WARN:
		Drivers::FbConsole::setColor(false, 0xff851b);
		FormatPrinter::print("WARN");
		break;
	case ERROR:
		Drivers::FbConsole::setColor(false, 0xff4136);
		FormatPrinter::print("ERROR");
		break;
	case FATAL:
		Drivers::FbConsole::setColor(false, 0xaa0000);
		FormatPrinter::print("FATAL");
		break;
	}
	Drivers::FbConsole::setColor(false, 0xb8b8b8);
	FormatPrinter::print("] ");
	FormatPrinter::printf(msg, args...);
	FormatPrinter::print('\n');
}

__NAMESPACE_END