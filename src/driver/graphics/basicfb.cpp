#include <driver/graphics/BasicFramebufferDriver>

using namespace EXOS::Driver::Graphics;

static uintptr_t fbAddress;
static uint32_t *fb;
static uint32_t width;
static uint32_t height;

BasicFramebufferDriver::BasicFramebufferDriver(uintptr_t _fb, uint32_t w, uint32_t h) {
  fbAddress = _fb;
  fb = (Pointer<uint32_t>)fbAddress;
  width = w;
  height = h;
}

void BasicFramebufferDriver::drawPixel(uint32_t x, uint32_t y, uint32_t rgb) {
  if (x >= width || y >= height) return;
  fb[y * width + x] = rgb;
}

uint32_t BasicFramebufferDriver::getWidth() {
  return width;
}

uint32_t BasicFramebufferDriver::getHeight() {
  return height;
}

uint32_t *BasicFramebufferDriver::getFramebuffer() {
  return fb;
}