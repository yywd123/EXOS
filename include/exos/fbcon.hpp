#pragma once

#include <display/displayBase.hpp>

__NAMESPACE_DECL(Drivers::FbConsole)

void
initialize();

void
print(uchar_t c);

void
print(const uchar_t *s);

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

__NAMESPACE_END