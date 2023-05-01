#pragma once

#include <display/DisplayUtils.hpp>

namespace EXOS::Utils::BmpViewer {
  typedef struct {
    uint16_t magic;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t bmpDataOffset;
    //  bmp信息头开始
    uint32_t bmpInfoSize;
    uint32_t frameWidth;
    uint32_t frameHeight;
    uint16_t reservedValue; //  必须为0x0001
    uint16_t bitsPerPixel;
    uint32_t compressionMode;
    uint32_t frameSize;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    uint32_t usedColorCount;
    uint32_t importantColorCount;
  } __attribute__((packed)) BmpHeader;

  static inline void displayBitmap(uint32_t x, uint32_t y, bool isTransparent, BmpHeader *header) {
    if (header->magic != 0x4d42 || header->bitsPerPixel != 24) return;
      uint8_t *data = (uint8_t*)header + header->bmpDataOffset;
      for (uint32_t yOffset = 0; yOffset < header->frameHeight; ++yOffset) {
      for (uint32_t xOffset = 0; xOffset < header->frameWidth; ++xOffset) {
        uint32_t offset = (yOffset * header->frameWidth + xOffset) * 3;
        uint8_t b = data[offset + 0];
        uint8_t g = data[offset + 1];
        uint8_t r = data[offset + 2];
        uint32_t color = (r << 16) | (g << 8) | b;
        if (isTransparent && color == 0) continue;
        Display::drawPixel(x + xOffset, y + header->frameHeight - 1 - yOffset, color);
      }
    }
  }
}