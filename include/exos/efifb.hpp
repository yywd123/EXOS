#pragma once

#include <display/displayBase.hpp>

__NAMESPACE_DECL(Drivers::EfiFb)

void __INIT
initializeEfiFb();

void
drawPixel(Display::Vec2D pos, RGBColor color);

void
drawRect(Display::Vec2D pos1, uint32_t w, uint32_t h, RGBColor color);

void
drawRect(Display::Vec2D pos1, Display::Vec2D pos2, RGBColor color);

void
drawRect(Display::BoundingBox boundingBox, RGBColor color);

void
drawLine(Display::Vec2D pos1, Display::Vec2D pos2, RGBColor color);

uint32_t
getWidth();

uint32_t
getHeight();

Display::Vec2D
getSize();

__NAMESPACE_END