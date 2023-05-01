#pragma once

#include <lib/OutputStream.hpp>
#include <lib/String.hpp>

namespace EXOS::Utils {
  class PrintWriter {
    
  public:
  OutputStream stream;
    PrintWriter(OutputStream os) {
      this->stream = os;
    }

    void print(const char* str) {
      this->stream->write(str, String::length(str));
    }

    void println(const char* str) {
      this->stream->write(str, String::length(str));
      this->stream->write(str_length(CRLF));
    }
  };
}