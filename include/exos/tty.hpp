#pragma once

#include <display/display.hpp>

__NAMESPACE_DECL(Miscs::TTY)

typedef struct {
	Display::Vec2D consolePos;
	Display::Vec2D consoleSize;
	Display::Vec2D cursorPos;
} TTYContext;

__NAMESPACE_END