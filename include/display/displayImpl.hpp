#pragma once

#include <display/displayBase.hpp>

namespace EXOS::Display {
  class DisplayImpl {
  private:
    Fn<void, uint32_t, uint32_t, RGBColor> drawPixelImpl;
    Fn<void, uint32_t, uint32_t, uint32_t, uint32_t, RGBColor> drawRectImpl;
  public:
    DisplayImpl(Fn<void, uint32_t, uint32_t, RGBColor> _drawPixelImpl, Fn<void, uint32_t, uint32_t, uint32_t, uint32_t, RGBColor> _drawRectImpl)
     : drawPixelImpl(_drawPixelImpl), drawRectImpl(_drawRectImpl) {}

    inline void
    drawPixel(uint32_t x, uint32_t y, RGBColor color) {
      if (drawPixelImpl) drawPixelImpl(x, y, color);
    }

    inline void
    drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, RGBColor color) {
      if (drawRectImpl) drawRectImpl(x, y, w, h, color);
    }

    __PLACEMENTNEW_DEFAULT
  };
}