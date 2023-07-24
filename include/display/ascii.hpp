#pragma once

#include <display/display.hpp>

__NAMESPACE_DECL(Display::Font::Ascii)

void
renderChar(Display::Vec2D pos, RGBColor backgroundColor, RGBColor fontColor, bool isZeroTranparent, char c);

__NAMESPACE_END