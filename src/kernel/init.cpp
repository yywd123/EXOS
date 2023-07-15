#include <platform.hpp>
#include <exos/efifb.hpp>
#include <exos/fbcon.hpp>
#include <exos/serial.hpp>
#include <hpet.hpp>
#include <utils/timeunit.hpp>
#include <exos/logger.hpp>
#include <exos/mm.hpp>

USE(EXOS::Drivers);
USE(EXOS::Utils);

void
initializeKernel() {
	Miscs::TTY::TTYContext ctx1{
			{0, 0},
			{FbConsole::getConsoleSize().x / 2, FbConsole::getConsoleSize().y},
			{0, 0}};

	Miscs::TTY::TTYContext ctx2{
			{FbConsole::getDefaultTTYContext().consoleSize.x / 2, 0},
			{FbConsole::getDefaultTTYContext().consoleSize.x / 2, FbConsole::getDefaultTTYContext().consoleSize.y / 2},
			{0, 0}};

	Miscs::TTY::TTYContext ctx3{
			{FbConsole::getDefaultTTYContext().consoleSize.x / 2, FbConsole::getDefaultTTYContext().consoleSize.y / 2},
			{FbConsole::getDefaultTTYContext().consoleSize.x / 2, FbConsole::getDefaultTTYContext().consoleSize.y / 2},
			{0, 0}};

	FbConsole::setTTYContext(&ctx1);
	Memory::initialize();

	Platform::MultiProcessor::initialize();
	Hpet::initialize();

	EfiFb::drawRect({0, 0}, EfiFb::getSize(), 0);
	ctx1.cursorPos = {0, 0};

	while(true) {
		int64_t i = TimeUnit::convert(TimeUnit::NANOSECONDS, Hpet::nanoTime(), TimeUnit::SECONDS);
		FbConsole::setTTYContext(&ctx1);
		FormatPrinter::printf("first TTY @\n", i);
		FbConsole::setTTYContext(&ctx2);
		FormatPrinter::printf("second TTY @\n", i);
		FbConsole::setTTYContext(&ctx3);
		FormatPrinter::printf("third TTY @\n", i);
		Hpet::sleep(TimeUnit::convert(TimeUnit::SECONDS, 1, TimeUnit::NANOSECONDS));
	}
}
