#pragma once

#include <display/displayBase.hpp>

__NAMESPACE_DECL(Drivers::EfiFb)

void __INIT
initializeEfiFb();

void
drawPixel(uint32_t x, uint32_t y, RGBColor color);

void
drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, RGBColor color);

uint32_t
getWidth();

uint32_t
getHeight();

__NAMESPACE_END