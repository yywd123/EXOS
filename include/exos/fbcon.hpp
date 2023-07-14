#pragma once

#include <display/displayBase.hpp>
#include <exos/tty.hpp>

__NAMESPACE_DECL(Drivers::FbConsole)

void
initialize();

void
print(char c);

void
print(const char *s);

void
setColor(RGBColor background, RGBColor foreground);

void
setColor(bool isBackground, RGBColor color);

void
setLineBase();

void
setCursorPos(Display::Vec2D pos);

Display::Vec2D
getCursorPos();

Display::Vec2D
getConsoleSize();

bool
setTTYContext(Miscs::TTY::TTYContext context);

Miscs::TTY::TTYContext
getDefaultTTYContext();

__NAMESPACE_END