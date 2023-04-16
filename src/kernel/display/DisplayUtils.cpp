#include <display/DisplayUtils>

using namespace EXOS::Driver::Graphics;

DisplayAdapter adapter;

namespace EXOS::Display {
  void setDisplayAdapter(DisplayAdapter a) {
    adapter = a;
  } 

  void drawPixel(uint32_t x, uint32_t y, uint32_t rgb) {
    adapter->drawPixel(x, y, rgb);
  }

  void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t rgb) {
    for (uint32_t i = 0; i < h; ++i) {
      for (uint32_t j = 0; j < w; ++j) adapter->drawPixel(x + j, y + i, rgb);
    }
  }
}