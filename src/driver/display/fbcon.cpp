#include <exos/fbcon.hpp>
#include <exos/efifb.hpp>
#include <efi/efi.h>

static Display::Vec2D cursorPos{0, 0};
static Display::Vec2D consoleSize;
static BitArray *fontType = nullptr;

static uint8_t *fontData = nullptr;

static RGBColor backgroundColor = 0x000000;
static RGBColor foregroundColor = 0xffffff;

__NAMESPACE_DECL(Drivers::FbConsole)

void
initialize() {
  consoleSize = EfiFb::getSize();
  consoleSize.x /= 8;
  consoleSize.y /= 16;

  fontType = new BitArray(consoleSize.x);

  EFI_FILE_HANDLE rootFs = efiOpenRootFs();
  uintptr_t address = 0x100000;
  efiReadFile(rootFs, L"\\font.sys", &address, EfiRuntimeServicesData, 0, 0);
  fontData = (uint8_t*)address;
}

void
print(uchar_t c) {
  switch (c) {
    case '\n':{
      cursorPos.x = 0;
      ++cursorPos.y;
      return;
    }
    case '\b':{
      if (cursorPos.x > 0)
        cursorPos.x -= fontType->get(cursorPos.x) ? 1 : 2;
      return;
    }
    case '\r':{
      cursorPos.x = 0;
      return;
    }
    default:{}
  }

  bool type = c > 0xff;
  uint8_t blockCount = type ? 2 : 1;
  if (cursorPos.x + blockCount >= consoleSize.x) {
    print(U'\n');
  }
  fontType->set(cursorPos.x, type);

  uint32_t offset = c * 32;
  for (uint8_t i = 0; i < 16; ++i) {
    for (uint8_t j = 0; j < blockCount; ++j) {
      for (uint8_t k = 0; k < 8 * blockCount; ++k) {
        if (fontData[offset + i * 2 + j] & (0x80 >> k))
          EfiFb::drawPixel({(cursorPos.x + j) * 8 + k, cursorPos.y * 16 + i}, foregroundColor);
        else if (backgroundColor != 0)
          EfiFb::drawPixel({(cursorPos.x + j) * 8 + k, cursorPos.y * 16 + i}, backgroundColor);
      }
    }
  }

  cursorPos.x += blockCount;
}

void
print(const uchar_t *s) {
  __iter(length(s)) {
    print(s[i]);
  }
}

void
print(char c) {
  print((uchar_t)c);
}

void
print(const char *s) {
  
}

void
setColor(RGBColor background, RGBColor foreground) {

}

void
setColor(bool isBackground, RGBColor color) {

}

void
setLineBase() {

}

void
setCursorPos(Display::Vec2D pos) {

}

__NAMESPACE_END