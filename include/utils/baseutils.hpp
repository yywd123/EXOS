#pragma once

template<typename T>
static inline size_t
length(const T *array) {
  size_t size;
  for (size = 0; array[size] != (T)0; ++size);
  return size;
}

template<typename P, typename T>
static inline void
getValueByOffset(P *base, uint64_t offset, T *value) {
  *value = *(T*)((uint8_t*)base + offset);
}

template<typename T>
static inline void
getValueByOffset(uintptr_t base, uint64_t offset, T *value) {
  getValueByOffset((void*)base, offset, value);
}

class BitArray {
private:
  size_t size;
  uint8_t *data;

public:
  BitArray(size_t arraySize) : data(new uint8_t[arraySize / 8 + (arraySize % 8 ? 1 : 0)]), size(arraySize) {}

  bool
  get(uint32_t index) {
    if (index < size) {
      return (data[index / 8] & BIT(index % 8) == 1);
    }
    return false;
  }

  void
  set(uint32_t index, bool value) {
    if (index < size) {
      data[index / 8] |= BIT(index % 8);
    }
  }
};