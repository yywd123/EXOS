#include <efi/efi.hpp>
#include <exos/efifb.hpp>
#include <exos/fbcon.hpp>

static uint8_t fontData[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00,
		0x12, 0x24, 0x24, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x12, 0x12, 0x12, 0x7E, 0x24, 0x24, 0x24, 0x7E, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00,
		0x00, 0x08, 0x3C, 0x4A, 0x4A, 0x48, 0x38, 0x0C, 0x0A, 0x0A, 0x4A, 0x4A, 0x3C, 0x08, 0x08, 0x00,
		0x00, 0x00, 0x44, 0xA4, 0xA8, 0xA8, 0xB0, 0x54, 0x1A, 0x2A, 0x2A, 0x4A, 0x44, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x30, 0x48, 0x48, 0x48, 0x50, 0x6E, 0xA4, 0x94, 0x98, 0x89, 0x76, 0x00, 0x00, 0x00,
		0x60, 0x20, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x00, 0x00,
		0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x10, 0x10, 0xD6, 0x38, 0x38, 0xD6, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x20, 0x20, 0x40, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00,
		0x00, 0x02, 0x04, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x00, 0x00,
		0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x08, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x02, 0x04, 0x08, 0x10, 0x20, 0x42, 0x7E, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3C, 0x42, 0x42, 0x02, 0x04, 0x18, 0x04, 0x02, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x04, 0x0C, 0x0C, 0x14, 0x24, 0x24, 0x44, 0x7F, 0x04, 0x04, 0x1F, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x78, 0x44, 0x02, 0x02, 0x42, 0x44, 0x38, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x18, 0x24, 0x40, 0x40, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x22, 0x1C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x7E, 0x42, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x38, 0x44, 0x42, 0x42, 0x42, 0x46, 0x3A, 0x02, 0x02, 0x24, 0x18, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x00,
		0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3C, 0x42, 0x42, 0x62, 0x04, 0x08, 0x08, 0x08, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x38, 0x44, 0x5A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x5C, 0x42, 0x3C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x10, 0x10, 0x18, 0x28, 0x28, 0x24, 0x3C, 0x44, 0x42, 0x42, 0xE7, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xF8, 0x44, 0x44, 0x44, 0x78, 0x44, 0x42, 0x42, 0x42, 0x44, 0xF8, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3E, 0x42, 0x42, 0x80, 0x80, 0x80, 0x80, 0x80, 0x42, 0x44, 0x38, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xF8, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 0xF8, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xFC, 0x42, 0x48, 0x48, 0x78, 0x48, 0x48, 0x40, 0x42, 0x42, 0xFC, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xFC, 0x42, 0x48, 0x48, 0x78, 0x48, 0x48, 0x40, 0x40, 0x40, 0xE0, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3C, 0x44, 0x44, 0x80, 0x80, 0x80, 0x8E, 0x84, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x88, 0xF0, 0x00,
		0x00, 0x00, 0xEE, 0x44, 0x48, 0x50, 0x70, 0x50, 0x48, 0x48, 0x44, 0x44, 0xEE, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xE0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x42, 0xFE, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xEE, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x54, 0x54, 0x54, 0x54, 0xD6, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xC7, 0x62, 0x62, 0x52, 0x52, 0x4A, 0x4A, 0x4A, 0x46, 0x46, 0xE2, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xFC, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40, 0x40, 0xE0, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0xB2, 0x4C, 0x38, 0x06, 0x00, 0x00,
		0x00, 0x00, 0xFC, 0x42, 0x42, 0x42, 0x7C, 0x48, 0x48, 0x44, 0x44, 0x42, 0xE3, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3E, 0x42, 0x42, 0x40, 0x20, 0x18, 0x04, 0x02, 0x42, 0x42, 0x7C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xFE, 0x92, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xE7, 0x42, 0x42, 0x44, 0x24, 0x24, 0x28, 0x28, 0x18, 0x10, 0x10, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xD6, 0x54, 0x54, 0x54, 0x54, 0x54, 0x6C, 0x28, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x24, 0x24, 0x42, 0xE7, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xEE, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x7E, 0x84, 0x04, 0x08, 0x08, 0x10, 0x20, 0x20, 0x42, 0x42, 0xFC, 0x00, 0x00, 0x00,
		0x1E, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x00, 0x00,
		0x00, 0x40, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x04, 0x02, 0x02, 0x00,
		0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00, 0x00,
		0x18, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
		0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x0C, 0x34, 0x44, 0x4C, 0x36, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xC0, 0x40, 0x40, 0x58, 0x64, 0x42, 0x42, 0x42, 0x64, 0x58, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x40, 0x40, 0x40, 0x22, 0x1C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x06, 0x02, 0x02, 0x3E, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3B, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x40, 0x42, 0x3C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x0C, 0x12, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x44, 0x44, 0x38, 0x40, 0x3C, 0x42, 0x42, 0x3C, 0x00,
		0x00, 0x00, 0x00, 0xC0, 0x40, 0x40, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x1C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x78, 0x00,
		0x00, 0x00, 0x00, 0xC0, 0x40, 0x40, 0x4E, 0x48, 0x50, 0x70, 0x48, 0x44, 0xEE, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x10, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x49, 0x49, 0x49, 0x49, 0x49, 0xED, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x62, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x64, 0x42, 0x42, 0x42, 0x64, 0x58, 0x40, 0xE0, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x02, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x32, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x42, 0x40, 0x3C, 0x02, 0x42, 0x7C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x12, 0x0C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3B, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB, 0x89, 0x4A, 0x5A, 0x54, 0x24, 0x24, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x24, 0x18, 0x18, 0x18, 0x24, 0x6E, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x10, 0x10, 0x60, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x44, 0x08, 0x10, 0x10, 0x22, 0x7E, 0x00, 0x00, 0x00,
		0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x03, 0x00, 0x00,
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x80,
		0xC0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00, 0x00,
		0x5A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static Display::Vec2D cursorPos{0, 0};
static Display::Vec2D consoleSize{0, 0};
static index_t lineBase = 0;

static RGBColor lastNonZeroBackgroundColor = 0;
static RGBColor backgroundColor = 0x000000;
static RGBColor foregroundColor = 0xffffff;

__NAMESPACE_DECL(Drivers::FbConsole)

void
initialize() {
	consoleSize = EfiFb::getSize();
	consoleSize.x /= 8;
	consoleSize.y /= 16;
}

void
print(char c) {
	switch(c) {
	case '\n': {
		if(cursorPos.y <= consoleSize.y)
			++cursorPos.y;
		else {
			__builtin_memcpy(
					EfiFb::getFramebuffer(),
					(void *)((uintptr_t)EfiFb::getFramebuffer() + EfiFb::getWidth() * 16 * 4),
					EfiFb::getWidth() * (EfiFb::getHeight() - 16) * 4);
			EfiFb::drawRect({0, (consoleSize.y - 1) * 16}, consoleSize.x * 8, 16, backgroundColor);
		}

		goto cr;
	}
	case '\b': {
		if(cursorPos.x <= lineBase) return;
		if(cursorPos.x != 0)
			--cursorPos.x;
		return;
	}
	case '\r': {
	cr:
		cursorPos.x = 0;
		return;
	}
	case '\x1b': {
		if(cursorPos.x <= lineBase) return;
		if(cursorPos.x != 0) {
			--cursorPos.x;
			EfiFb::drawRect({cursorPos.x * 8, cursorPos.y * 16}, 8, 16, lastNonZeroBackgroundColor);
		}
		return;
	}
	default: {
	}
	}

	if(cursorPos.x + 1 > consoleSize.x) {
		print('\n');
	}

	uint32_t offset = c < 0x20 ? 0 : (c - 0x20) * 16;
	for(uint8_t i = 0; i < 16; ++i) {
		for(uint8_t k = 0; k < 8; ++k) {
			if(fontData[offset + i] & (0x80 >> k))
				EfiFb::drawPixel({cursorPos.x * 8 + k, cursorPos.y * 16 + i}, foregroundColor);
			else {
				if(backgroundColor != 0)
					EfiFb::drawPixel({cursorPos.x * 8 + k, cursorPos.y * 16 + i}, backgroundColor);
			}
		}
	}

	++cursorPos.x;
}

void
print(const char *s) {
	__iter(length(s)) {
		print(s[i]);
	}
}

void
setColor(RGBColor background, RGBColor foreground) {
	if(backgroundColor != 0) lastNonZeroBackgroundColor = backgroundColor;
	backgroundColor = background;
	foregroundColor = foreground;
}

void
setColor(bool isBackground, RGBColor color) {
	if(isBackground) {
		if(backgroundColor != 0) lastNonZeroBackgroundColor = backgroundColor;
		backgroundColor = color;
	} else
		foregroundColor = color;
}

void
setLineBase() {
	lineBase = cursorPos.x != 0 ? cursorPos.x - 1 : 0;
}

void
setLineBase(index_t base) {
	lineBase = base >= consoleSize.x ? consoleSize.x - 1 : base;
}

void
setCursorPos(Display::Vec2D pos) {
	cursorPos.x = pos.x >= consoleSize.x ? consoleSize.x - 1 : pos.x;
	cursorPos.y = pos.y >= consoleSize.y ? consoleSize.y - 1 : pos.y;
}

Display::Vec2D
getConsoleSize() {
	return consoleSize;
}

__NAMESPACE_END