#pragma once

#include <display/displayBase.hpp>

__NAMESPACE_DECL(Drivers::FbConsole)

void
initialize();

void
renderChar(Display::Vec2D pos, char c);

void
print(char c);

void
print(const char *s);

void
setColor(RGBColor background, RGBColor foreground);

void
setColor(bool isBackground, RGBColor color);

void
setIsZeroTransparent(bool value);

void
setLineBase();

void
setCursorPos(Display::Vec2D pos);

Display::Vec2D
getConsoleSize();

__NAMESPACE_END