#include <exos/fbcon.hpp>
#include <exos/efifb.hpp>
#include <efi/efi.h>

static Display::Vec2D cursorPos{0, 0};
static Display::Vec2D consoleSize;
static BitArray *fontType = nullptr;
static index_t fontTypeIndex = 0;
static index_t lineBase = 0;

static uint8_t *fontData = nullptr;

static RGBColor lastNonZeroBackgroundColor = 0x39c5bb;
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

static inline void printUInt(uint64_t i, uint8_t n) {  //  统一打印十六进制
  if (i == 0) {
    for(int i = 0; i < n; ++i) efiPuts(L"0");
    return;
  } 
  const char *digits = "0123456789abcdef";
  wchar_t buf[17] = {0};
  for (uint8_t j = 16; j != 0; --j) {
    buf[j - 1] = digits[i & 0xf];
    i >>= 4;
  }

  const wchar_t *p = &buf[16 - n];

  efiPuts(p);
}

void
print(wchar_t c) {
  printUInt(c, 8);
  efiPuts(L"\n");
  switch (c) {
    case '\n':{
      fontTypeIndex = 0;
      cursorPos.x = 0;
      ++cursorPos.y;
      return;
    }
    case '\b':{
      if (fontTypeIndex <= lineBase) return;
      if (fontTypeIndex != 0)
        cursorPos.x -= fontType->get(--fontTypeIndex) ? 2 : 1;
      return;
    }
    case '\r':{
      cursorPos.x = 0;
      return;
    }
    case '\x1b':{
      if (fontTypeIndex != 0) {
        uint8_t lastBlockCount = fontType->get(--fontTypeIndex) ? 2 : 1;
        cursorPos.x -= lastBlockCount;
        EfiFb::drawRect({cursorPos.x * 8, cursorPos.y * 16}, lastBlockCount * 8, 16, backgroundColor);
      }
      return;
    }
    default:{}
  }

  bool type = c > 0xff;
  uint8_t blockCount = type ? 2 : 1;
  if (cursorPos.x + blockCount >= consoleSize.x) {
    print(L'\n');
  }

  uint16_t emptyCount = 0;

  uint32_t offset = c * 32;
  for (uint8_t i = 0; i < 16; ++i) {
    for (uint8_t j = 0; j < blockCount; ++j) {
      for (uint8_t k = 0; k < 8; ++k) {
        if (fontData[offset + i * 2 + j] & (0x80 >> k))
          EfiFb::drawPixel({(cursorPos.x + j) * 8 + k, cursorPos.y * 16 + i}, foregroundColor);
        else {
          ++emptyCount;
          if (backgroundColor != 0)
            EfiFb::drawPixel({(cursorPos.x + j) * 8 + k, cursorPos.y * 16 + i}, backgroundColor);
        }
      }
    }
  }

  if (emptyCount == 256) return;

  fontType->set(fontTypeIndex++, type);
  cursorPos.x += blockCount;
}

void
print(const wchar_t *s) {
  __iter(length(s)) {
    print(s[i]);
  }
}

void
print(char c) {
  print((wchar_t)c);
}

int
utf8ToUnicode(const char* pInput, char** ppOutput) {  //https://blog.csdn.net/u013815973/article/details/24441437
  int outputSize = 0; //记录转换后的Unicode字符串的字节数

  *ppOutput = new char[length(pInput) * 2];  //为输出字符串分配足够大的内存空间
  __builtin_memset(*ppOutput, 0, length(pInput) * 2);
  char *tmp = *ppOutput; //临时变量，用于遍历输出字符串

  while (*pInput) {
    if (*pInput > 0x00 && *pInput <= 0x7F) { //处理单字节UTF8字符（英文字母、数字）
      *tmp = *pInput;
      tmp++;
      *tmp = 0; //小端法表示，在高地址填补0
    } else if (((*pInput) & 0xE0) == 0xC0) { //处理双字节UTF8字符
      char high = *pInput;
      pInput++;
      char low = *pInput;

      if ((low & 0xC0) != 0x80) { //检查是否为合法的UTF8字符表示
        return -1; //如果不是则报错
      }

      *tmp = (high << 6) + (low & 0x3F);
      tmp++;
      *tmp = (high >> 2) & 0x07;
    } else if (((*pInput) & 0xF0) == 0xE0) { //处理三字节UTF8字符
      char high = *pInput;
      pInput++;
      char middle = *pInput;
      pInput++;
      char low = *pInput;

      if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80)) {
        return -1;
      }

      *tmp = (middle << 6) + (low & 0x7F);
      tmp++;
      *tmp = (high << 4) + ((middle >> 2) & 0x0F); 
    } else { //对于其他字节数的UTF8字符不进行处理
      return -1;
    }

    pInput ++;
    tmp ++;
    outputSize += 2;
  }

  *tmp = 0;
  tmp++;
  *tmp = 0;

  return outputSize;
}

void
print(const char *s) {
  char *ustr;
  utf8ToUnicode(s, &ustr);
  print((const wchar_t*)ustr);
}

void
setColor(RGBColor background, RGBColor foreground) {
  if (backgroundColor != 0) lastNonZeroBackgroundColor = backgroundColor;
  backgroundColor = background;
  foregroundColor = foreground;
}

void
setColor(bool isBackground, RGBColor color) {
  if (isBackground) {
    if (backgroundColor != 0) lastNonZeroBackgroundColor = backgroundColor;
    backgroundColor = color;
  }
  else foregroundColor = color;
}

void
setLineBase() {
 lineBase = fontTypeIndex != 0 ? fontTypeIndex - 1 : 0;
}

void
setLineBase(index_t base) {
  lineBase = base >= consoleSize.x ? consoleSize.x : base;
}

void
setCursorPos(Display::Vec2D pos) {
  cursorPos.x = pos.x >= consoleSize.x ? consoleSize.x - 1 : pos.x;
  cursorPos.y = pos.y >= consoleSize.y ? consoleSize.y - 1 : pos.y;
}

__NAMESPACE_END