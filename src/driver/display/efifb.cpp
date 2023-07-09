#include <efi/efi.h>
#include <exos/efifb.hpp>
#include <utils/math.hpp>

USE(EXOS::Drivers);

static RGBColor *framebuffer = nullptr;
static Display::Vec2D framebufferSize;

__NAMESPACE_DECL(Drivers::EfiFb)

void __INIT
initialize() {
  Guid gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP = (EFI_GRAPHICS_OUTPUT_PROTOCOL*)efiLocateProtocol(&gopGuid, nullptr);

  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *modeInfo;
  uint64_t infoSize;
  uint64_t pixelCount = GOP->Mode->Info->PixelsPerScanLine * GOP->Mode->Info->VerticalResolution;
  uint32_t modeIndex = GOP->Mode->Mode;

  for (uint32_t i = 0; i < GOP->Mode->MaxMode; ++i) {
    eficall(GOP->QueryMode, GOP, i, &infoSize, &modeInfo);
    if ((modeInfo->PixelsPerScanLine * modeInfo->VerticalResolution) >= pixelCount) {
      pixelCount = modeInfo->PixelsPerScanLine * modeInfo->VerticalResolution;
      modeIndex = i;
    }
  }
  eficall(GOP->SetMode, GOP, modeIndex);

  GOP = (EFI_GRAPHICS_OUTPUT_PROTOCOL*)efiLocateProtocol(&gopGuid, nullptr);

  framebuffer = (RGBColor*)GOP->Mode->FrameBufferBase;
  framebufferSize.x = GOP->Mode->Info->PixelsPerScanLine;
  framebufferSize.y = GOP->Mode->Info->VerticalResolution;
}

void
drawPixel(Display::Vec2D pos, RGBColor color) {
  if (pos.x < framebufferSize.x && pos.y < framebufferSize.y) framebuffer[pos.y * framebufferSize.x + pos.x] = color;
}

void
drawRect(Display::Vec2D pos, uint32_t w, uint32_t h, RGBColor color) {
  uint32_t x = pos.x;
  uint32_t y = pos.y;

  for (uint32_t i = 0; i < h; ++i) {
    for (uint32_t j = 0; j < w; ++j) {
      if (x + j < framebufferSize.x && y + i < framebufferSize.y) drawPixel({x + j, y + i}, color);
    }
  }
}

void
drawRect(Display::Vec2D pos1, Display::Vec2D pos2, RGBColor color) {
  uint32_t x = min(pos1.x, pos2.x);
  uint32_t y = min(pos1.y, pos2.y);
  uint32_t w = max(pos1.x, pos2.x) - x;
  uint32_t h = max(pos1.y, pos2.y) - y;

  for (uint32_t i = 0; i < h; ++i) {
    for (uint32_t j = 0; j < w; ++j) {
      if (x + j < framebufferSize.x && y + i < framebufferSize.y) drawPixel({x + j, y + i}, color);
    }
  }
}

void
drawRect(Display::BoundingBox boundingBox, RGBColor color) {
  drawRect(Display::Vec2D{boundingBox.x1, boundingBox.y1}, Display::Vec2D{boundingBox.x2, boundingBox.y2}, color);
}

void
drawLine(Display::Vec2D pos1, Display::Vec2D pos2, RGBColor color) {
  uint32_t x1 = pos1.x;
  uint32_t y1 = pos1.y;
  uint32_t x2 = pos2.x;
  uint32_t y2 = pos2.y;

  //https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
  int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
  int dy = abs(y2-y1), sy = y1<y2 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;

  while (1) {
    drawPixel({x1, y1}, color);
    if (x1==x2 && y1==y2) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x1 += sx; }
    if (e2 < dy) { err += dx; y1 += sy; }
  }
}

uint32_t
getWidth() {
  return framebufferSize.x;
}

uint32_t
getHeight() {
  return framebufferSize.y;
}

Display::Vec2D
getSize() {
  return framebufferSize;
}

void
*getFramebuffer() {
  return framebuffer;
}

__NAMESPACE_END