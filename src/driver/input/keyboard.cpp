#include <arch/arch.h>
#include <input/keyboard.h>
#include <lib/Logger.hpp>
#include <lib/Stack.hpp>
#include <lib/EventManager.hpp>
#include <driver/graphics/FramebufferConsole.hpp>
#include <lib/BmpViewer.hpp>

using namespace EXOS::Utils;
using namespace EXOS::Driver;

static Stack<char> *keyboardInputStack = nullptr;

typedef union {
  uint8_t rawData;
  struct {
    uint8_t scanCode : 7;
    bool isReleased : 1;
  };
} PS2ScanCode;

char keytable[0x54] = {
    0,    0,    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',  '+',
    '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{',  '}',
    10,   0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
    0,    '|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,    '*',
    0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
    0,    '7',  'D', '8', '-', '4', '5', '6', '+', '1', '2', '3', '0',  '.'};
char keytable1[0x54] = {
    0,    0,    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
    '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',  ']',
    10,   0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,    '*',
    0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
    0,    '7',  '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0',  '.'};

static uint8_t overrideCount = 0;

static uint16_t ps2GetDisplayableKeyCode(uint8_t keyData) {
  if (keyData == 0x2a && overrideCount) {
    overrideCount = 2;
    return 0xffff;
  }
  if (overrideCount) {
    --overrideCount;
    return 0xffff;
  }
  PS2ScanCode code = {.rawData = keyData};
  if (keyData == 0xe0) {
    overrideCount = 1;
    return 0xffff;
  }
  if (keyData == 0xe1) {
    overrideCount = 5;
    return 0xffff;
  }
  return (uint16_t)code.scanCode | (code.isReleased ? BIT(15) : 0);
}

bool capsLock = false;
bool shift = false;

uint64_t sleepCountDown = 0;
void sleep(uint64_t ms) {
  maskIRQ(0, true);
  sleepCountDown = (ms + 1) / 10;
  ASM("sti");
  while (sleepCountDown > 0) ASM("nop");
  maskIRQ(0, false);
}

void nextLine() {
  if (keyboardInputStack->size() == 1) {
    keyboardInputStack->pop();
    return;
  }
  char lineBuf[0x201] = {0};
 
  size_t size = keyboardInputStack->size();  //不能直接放到iter()里面的原因:push/pop之后 值会更新
  iter(size) {
    lineBuf[size - i - 1] = keyboardInputStack->pop();
  }

  if (__builtin_memcmp(lineBuf, str_length("qwq")) == 0) {
    Logger::print("awa\n");
  } else if (__builtin_memcmp(lineBuf, str_length("info")) == 0) {
    Graphics::FramebufferConsole::setFontColor(0x39c5bb);
    Logger::print("EXOS ");
    Graphics::FramebufferConsole::setFontColor(0xb8b8b8);
    Logger::print(EXOS_VERSION);
    Logger::print('\n');
    Logger::print("Developing by yywd_123(");
    Graphics::FramebufferConsole::setFontColor(0x228b22);
    Logger::print("https://space.bilibili.com/689917252");
    Graphics::FramebufferConsole::setFontColor(0xb8b8b8);
    Logger::print(")\nGit repository: ");
    Graphics::FramebufferConsole::setFontColor(0x228b22);
    Logger::print("https://gitee.com/yywd123/EXOS\n");
    Graphics::FramebufferConsole::setFontColor(0xb8b8b8);
  } else if (__builtin_memcmp(lineBuf, str_length("video")) == 0) {
    uint64_t i = 0;
    while(i <= 0xffffffffffffffff) {
      if (BmpViewer::displayBitmap(0, 0, false, (BmpViewer::BmpHeader*)(0x600000 + i++ * 230454))) break;
      sleep(100);
    }
  } else {
    Logger::print("command not found:");
    Logger::print((const char*)lineBuf);
  }
}

void updateKeyboardState(KeyboardType type, uint8_t keyData) {
 //Logger::log(Logger::INFO, "raw keydata 0x@", keyData);
  if (!keyboardInputStack) {
    keyboardInputStack = new Stack<char>(512);
  }
  uint16_t c = 0;
  switch (type) {
    match(PS2, c = ps2GetDisplayableKeyCode(keyData))
  }

  if (c == 0xffff) return;

  if ((c & 0xff) == KEY_CAPSLOCK && (c & BIT(15))) {
    capsLock = !capsLock;
  } else if ((c & 0xff) == KEY_LSHIFT || (c & 0xff) == KEY_RSHIFT) {
    shift = ((c & BIT(15)) == 0);
  }
  if (!(c & BIT(15) && (c & 0xff) < 0x54)) {
    char tmp = (capsLock != shift ? keytable : keytable1)[(c & 0xff)];
    if (tmp == '\b') keyboardInputStack->pop();
    else keyboardInputStack->push(tmp);
    Logger::print(tmp);
    if (tmp == '\n') {
      nextLine();
      Logger::print("> ");
      Graphics::FramebufferConsole::setBaselineAtCurrentPos();
    }
  }
}