#include <efi/efi.hpp>
#include <exos/efifb.hpp>
#include <exos/fbcon.hpp>
#include <display/ascii.hpp>

//	经过我的慎重思考 决定把fbcon当成临时的(所以砍掉一堆不必要的功能
//	因为孩子想做gui(qwq

static Display::Vec2D cursorPos{0, 0};
static Display::Vec2D consoleSize{0, 0};
static index_t lineBase = 0;

static RGBColor backgroundColor = 0x000000;
static RGBColor foregroundColor = 0xffffff;
static bool isZeroTranparent = false;

__NAMESPACE_DECL(Drivers::FbConsole)

void __INIT
initialize() {
	consoleSize = EfiFb::getSize();
	consoleSize.x /= 8;
	consoleSize.y /= 16;
}

void
print(char c) {
	switch(c) {
	case '\n': {
		if(cursorPos.y < consoleSize.y - 1)
			++cursorPos.y;
		else {
			EfiFb::copyToFramebuffer(
					EfiFb::getFramebuffer(),
					{0, 16},
					{0, 0},
					{consoleSize.x * 8, (consoleSize.y - 1) * 16});

			EfiFb::drawRect({0, (consoleSize.y - 1) * 16}, consoleSize.x * 8, 16, backgroundColor);
		}
		lineBase = 0;

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
	case '\t': {
		uint8_t n = 0;

		if((cursorPos.x + 1) % 4 == 0) {
			n = 4;
		} else {
			n = 4 - (cursorPos.x + 1) % 4;
		}

		if(cursorPos.x + n >= consoleSize.x) {
			cursorPos.x = consoleSize.x;
		} else
			cursorPos.x += n;
	}
	default: {
	}
	}

	if(cursorPos.x + 1 > consoleSize.x) {
		print('\n');
	}

	Display::Font::Ascii::renderChar({cursorPos.x * 8, cursorPos.y * 16}, backgroundColor, foregroundColor, isZeroTranparent, c);

	++cursorPos.x;
}

void
print(const char *s) {
	_iter(length(s)) {
		print(s[i]);
	}
}

void
setColor(RGBColor background, RGBColor foreground) {
	backgroundColor = background;
	foregroundColor = foreground;
}

void
setColor(bool isBackground, RGBColor color) {
	if(isBackground) {
		backgroundColor = color;
	} else
		foregroundColor = color;
}

void
setIsZeroTransparent(bool value) {
	isZeroTranparent = value;
}

void
setLineBase() {
	lineBase = cursorPos.x;
}

void
setLineBase(index_t base) {
	lineBase = base >= consoleSize.x ? consoleSize.x : base;
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