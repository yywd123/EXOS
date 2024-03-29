#pragma once

#include <driver/graphics/DisplayAdapter.hpp>

namespace EXOS::Display {
  void setDisplayAdapter(EXOS::Driver::Graphics::DisplayAdapter a);
  EXOS::Driver::Graphics::DisplayAdapter getDisplayAdapter();
  void drawPixel(uint32_t x, uint32_t y, uint32_t rgb);
  void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t rgb);
}