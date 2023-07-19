#include <platform.hpp>
#include <exos/efifb.hpp>
#include <exos/fbcon.hpp>
#include <exos/serial.hpp>
#include <hpet.hpp>
#include <utils/timeunit.hpp>
#include <exos/logger.hpp>
#include <exos/mm.hpp>
#include <exos/cmos.hpp>

USE(EXOS::Drivers);
USE(EXOS::Utils);

extern void
dumpAllocationInfo();

void
drawCloseIcon(Display::Vec2D pos) {
	EfiFb::drawLine(pos + Display::Vec2D{6, 6}, pos + Display::Vec2D{14, 14}, 0xffffff);
	EfiFb::drawLine(pos + Display::Vec2D{14, 6}, pos + Display::Vec2D{6, 14}, 0xffffff);
}

void
drawWindow(Display::Vec2D pos, Display::Vec2D size, const char *title) {
	EfiFb::drawRect(pos, pos + Display::Vec2D{size.x, 20}, 0x31363b);
	EfiFb::drawRect(pos + Display::Vec2D{0, 20}, pos + size, 0x2a2e32);
	EfiFb::drawRect(pos + Display::Vec2D{2, 2}, pos + Display::Vec2D{18, 18}, 0xffffff);

	uint32_t titleStartX = (size.x - length(title) * 8) / 2;
	FbConsole::setIsZeroTransparent(true);
	__iter(length(title)) {
		FbConsole::renderChar(pos + Display::Vec2D{titleStartX + i * 8, 2}, title[i]);
	}

	drawCloseIcon(pos + Display::Vec2D{size.x - 20, 0});
}

void
initializeKernel() {
	Memory::initialize();

	Platform::MultiProcessor::initialize();
	HPET::initialize();
	CMOS::setTimeOffset(8, 0);	//	CST = UTC+8
	CMOS::initialize();

	EfiFb::drawRect({0, 0}, EfiFb::getSize(), 0x39c5bb);
	drawWindow({10, 10}, {400, 150}, "test window");

	// dumpAllocationInfo();

	// while(true) {
	// 	FbConsole::setCursorPos({0, 0});
	// 	Logger::printf("@\r", CMOS::getTime());
	// 	HPET::sleep(TimeUnit::convert(TimeUnit::SECONDS, 1, TimeUnit::NANOSECONDS));
	// }
}
