#include <platform.hpp>
#include <exos/fbcon.hpp>
#include <exos/serial.hpp>
#include <hpet.hpp>
#include <utils/timeunit.hpp>
#include <exos/logger.hpp>

USE(EXOS::Drivers);
USE(EXOS::Utils);

void
initializeKernel() {
	Platform::MultiProcessor::initBootstrapProcessor();
	Hpet::initialize();

	while(true) {
		FbConsole::print("qwq\n");
		Hpet::sleep(TimeUnit::convert(TimeUnit::SECONDS, 1, TimeUnit::NANOSECONDS));
	}
}
