#pragma once

template<typename T>
static inline size_t
length(const T *array) {
	size_t size;
	for(size = 0; array[size] != (T)0; ++size)
		;
	return size;
}

template<typename P, typename T>
static inline void
getValueByOffset(P *base, uint64_t offset, T *value) {
	*value = *(T *)((uint8_t *)base + offset);
}

template<typename T>
static inline void
getValueByOffset(uintptr_t base, uint64_t offset, T *value) {
	getValueByOffset((void *)base, offset, value);
}

class BitArray {
private:
	size_t size;
	uint8_t *data;

public:
	BitArray(size_t arraySize) :
		data(new uint8_t[__divceil(arraySize, 8)]), size(arraySize) {
		clear();
	}
	BitArray(void *buf, size_t arraySize) :
		data((uint8_t *)buf), size(arraySize) {
		clear();
	}

	bool
	get(uint32_t index) {
		if(index < size) {
			return (data[index / 8] & BIT(index % 8));
		}
		return false;
	}

	void
	set(uint32_t index, bool value) {
		if(index < size) {
			if(value)
				data[index / 8] |= BIT(index % 8);
			else
				data[index / 8] &= ~BIT(index % 8);
		}
	}

	size_t
	getSize() {
		return size;
	}

	void
	clear() {
		__builtin_memset(data, 0, __divceil(size, 8));
	}
};