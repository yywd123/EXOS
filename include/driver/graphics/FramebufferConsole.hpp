#pragma once

#include <driver/graphics/DisplayAdapter.hpp>

namespace EXOS::Driver::Graphics::FramebufferConsole {
  void init(DisplayAdapter adapter);
  void print(char c);
  void print(const char *s);
  void setFontColor(uint32_t rgb);
  void setBaselineAtCurrentPos();
}