#include <platform/platform.hpp>
#include <exos/efifb.hpp>
#include <exos/logger.hpp>
#include <utils/timeunit.hpp>
#include <display/ascii.hpp>

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
	__iter(length(title)) {
		Display::Font::Ascii::renderChar(pos + Display::Vec2D{titleStartX + (uint32_t)i * 8, 2}, 0, 0xffffff, true, title[i]);
	}

	drawCloseIcon(pos + Display::Vec2D{size.x - 20, 0});
}

void __INIT
initializeKernel() {
	Platform::initialize();
	// while(true) {
	// 	Logger::printf("time [@]\r", CMOS::getTime());
	// 	HPET::sleep(TimeUnit::convert(TimeUnit::SECONDS, 1, TimeUnit::NANOSECONDS));
	// }

	// EfiFb::drawRect({0, 0}, EfiFb::getSize(), 0x39c5bb);
	// EfiFb::drawRect({0, EfiFb::getSize().y - 40}, EfiFb::getSize(), 0x2a2e32);
	// drawWindow({10, 10}, {400, 150}, "test window");

	// dumpAllocationInfo();

	// while(true) {
	// 	FbConsole::setCursorPos({0, 0});
	// 	Logger::printf("@\r", CMOS::getTime());
	// 	HPET::sleep(TimeUnit::convert(TimeUnit::SECONDS, 1, TimeUnit::NANOSECONDS));
	// }
}
