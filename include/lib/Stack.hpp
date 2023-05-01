#pragma once

#include <utils.hpp>

namespace EXOS::Utils {
  template<typename T>
  class Stack {
  private:
    T *array;
    size_t maxSize;
    uint32_t index = 0;
  public:
    Stack(size_t size) {
      this->maxSize = size;
      this->array = new T[size];
    }

    bool isEmpty() {
      return (this->index == 0);
    }

    bool isFull() {
      return (this->maxSize == this->index);
    }

    void push(T obj) {
      if (!isFull()) {
        this->array[this->index++] = obj;
      }
    }

    T pop() {
      if (isEmpty()) return (T)0;
      else return this->array[--this->index];
    }

    size_t size() {
      return index;
    }

    size_t capacity() {
      return maxSize;
    }
  };
}