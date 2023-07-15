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
	FbConsole::setTTYContext(
			Miscs::TTY::TTYContext{{0, 0},
														 {FbConsole::getConsoleSize().x / 2, FbConsole::getConsoleSize().y},
														 {0, 0}});
	Memory::initialize();

	Platform::MultiProcessor::initialize();
	Hpet::initialize();

	bool stat = FbConsole::setTTYContext(
			Miscs::TTY::TTYContext{{FbConsole::getDefaultTTYContext().consoleSize.x / 2, 0},
														 {FbConsole::getDefaultTTYContext().consoleSize.x / 2, FbConsole::getDefaultTTYContext().consoleSize.y},
														 {0, 0}});

	while(true) {
		FormatPrinter::printf("\nhpet enabled since @s ago", (int64_t)TimeUnit::convert(TimeUnit::NANOSECONDS, Hpet::nanoTime(), TimeUnit::SECONDS));
		Hpet::sleep(TimeUnit::convert(TimeUnit::SECONDS, 1, TimeUnit::NANOSECONDS));
	}
}
