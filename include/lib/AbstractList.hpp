#pragma once

#include <types.hpp>

namespace EXOS::Utils {
  template<typename T>
  class _AbstractList {
  public:
    virtual bool isEmpty() = 0;
    virtual uint32_t indexOf(T obj) = 0;
    virtual bool contains(T obj) = 0;
    virtual void add(T obj) = 0;
    virtual void set(uint32_t i, T obj) = 0;
    virtual T get(uint32_t i) = 0;
    virtual void remove(uint32_t i) = 0;
    virtual size_t getSize() = 0;
  };

  template<typename T>
  using AbstractList = _AbstractList<T>*;
}