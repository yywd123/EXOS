#pragma once

#include <lib/OutputStream.hpp>
#include <lib/guid.h>
#include <driver/graphics/FramebufferConsole.hpp>

namespace EXOS::Utils::Logger {
  typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
  } LogLevel;

  extern const char *sectionName;
  extern Utils::OutputStream loggerStream;

  void beginSection(const char *name);
  void endSection();
  void saveAndBeginSection(const char *name);
  void restoreSection();

  static inline void print(char c) {
    if (!loggerStream) return;
    loggerStream->write(c);
  }

  static inline void print(const char *s) {
    for (uint64_t i = 0; s[i] != 0; ++i) {
			print(s[i]);
		}
  }

  static inline void printInt(int64_t i) {
    if (i < 0) {
      print('-');
      i = -i;
    }
    if (i > 9) {
      printInt(i / 10);
    }
    print('0' + i % 10);
  }

  static inline void printUInt(uint64_t i, uint8_t n) {  //  统一打印十六进制
    if (i == 0) {
      iter(n) print('0');
      return;
    } 
    const char *digits = "0123456789abcdef";
    char buf[17] = {0};
    for (uint8_t j = 16; j != 0; --j) {
      buf[j - 1] = digits[i & 0xf];
      i >>= 4;
    }

    const char *p = &buf[16 - n];

    print(p);
  }

  static inline void print(uint8_t byte) {
    printUInt(byte, 2);
  }

  static inline void print(int16_t i) {
    printInt(i);
  }

  static inline void print(uint16_t i) {
    printUInt(i, 4);
  }

  static inline void print(int32_t i) {
    printInt(i);
  }

  static inline void print(uint32_t i) {
    printUInt(i, 8);
  }

  static inline void print(int64_t i) {
    printInt(i);
  }

  static inline void print(size_t i) {
    printUInt(i, 16);
  }

  static inline void print(uint64_t i) {
    printUInt(i, 16);
  }

  static inline void print(bool b) {
    if (b) print("true");
    else print("false");
  }

  static inline void print(GUID guid) {
    print(guid.field1);
    print('-');
    print(guid.field2);
    print('-');
    print(guid.field3);
    print('-');
    iter(8) {
      print(guid.field4[i]);
    }
  }

  template<typename T>
  static inline void print(T *p) {
    if (p == nullptr) {
      print("ptr[nullptr]");
      return;
    }
    print("ptr[0x");
    printUInt((uint64_t)p, 16);
    print(']');
  }

  template<typename T>
  static inline void print(T) {
    print("[unknown type]");
  }

  static inline void log(const char *msg) {
    while (*msg) {
      if (*msg == '@' && *(++msg) != '@') {
        print("???");
      }
      print(*msg++);
    }
  }

  template<typename T, typename... Args>
  static inline void log(const char *msg, const T value, const Args &...args) {
    while (*msg) {
      if (*msg == '@' && *(++msg) != '@') {
        print(value);
        log(msg, args...);
        return;
      }
      print(*msg++);
    }
  }

  template<typename... Args>
  static inline void log(LogLevel level, const char *msg, const Args &...args) {
    EXOS::Driver::Graphics::FramebufferConsole::setFontColor(0xb8b8b8);
    print('[');
    switch (level) {
      default:
      case DEBUG:
        EXOS::Driver::Graphics::FramebufferConsole::setFontColor(0x018070);
        print("DEBUG");
        break;
      case INFO:
        EXOS::Driver::Graphics::FramebufferConsole::setFontColor(0x2ecc40);
        print("INFO");
        break;
      case WARN:
        EXOS::Driver::Graphics::FramebufferConsole::setFontColor(0xff851b);
        print("WARN");
        break;
      case ERROR:
        EXOS::Driver::Graphics::FramebufferConsole::setFontColor(0xff4136);
        print("ERROR");
        break;
      case FATAL:
        EXOS::Driver::Graphics::FramebufferConsole::setFontColor(0xaa0000);
        print("FATAL");
        break;
    }
    EXOS::Driver::Graphics::FramebufferConsole::setFontColor(0xb8b8b8);
    print(']');
    EXOS::Driver::Graphics::FramebufferConsole::setFontColor(0x60be60);
    print('(');
    print(sectionName);
    print(") ");
    EXOS::Driver::Graphics::FramebufferConsole::setFontColor(0xb8b8b8);
    log(msg, args...);
    print('\n');
  }
}