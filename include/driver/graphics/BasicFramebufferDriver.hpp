#pragma once

#include <driver/graphics/DisplayAdapter.hpp>

namespace EXOS::Driver::Graphics {
  class BasicFramebufferDriver : public _DisplayAdapter {
  public:
    BasicFramebufferDriver(uintptr_t fb, uint32_t w, uint32_t h);
    void drawPixel(uint32_t x, uint32_t y, uint32_t rgb);
    uint32_t getWidth();
    uint32_t getHeight();
    uint32_t *getFramebuffer();
  };
}