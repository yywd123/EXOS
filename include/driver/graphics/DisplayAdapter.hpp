#pragma once

#include <utils.hpp>

namespace EXOS::Driver::Graphics {
  class _DisplayAdapter {
  public:
    virtual void drawPixel(uint32_t x, uint32_t y, uint32_t rgb) = 0;
    virtual uint32_t getWidth() = 0;
    virtual uint32_t getHeight() = 0;
    virtual uint32_t *getFramebuffer() = 0;
  };

  using DisplayAdapter = _DisplayAdapter*;
}