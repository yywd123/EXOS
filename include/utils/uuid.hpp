#pragma once

#include <utils/string.hpp>

typedef struct UUID {
public:
	uint32_t field1;
	uint16_t field2;
	uint16_t field3;
	uint8_t field4[8];

	struct UUID
	fromString(const char *str) {
		const char *s = String::toLowerCase(str);
		this->field1 = String::parseHexInt(s);
		this->field2 = String::parseHexShort(s + 9);
		this->field3 = String::parseHexShort(s + 14);
		this->field4[0] = String::parseHexByte(s + 19);
		this->field4[1] = String::parseHexByte(s + 21);
		_iter(6) {
			this->field4[i + 2] = String::parseHexByte(s + 24 + i * 2);
		}

		return *this;
	}
} __packed UUID;

static inline bool
uuidCompare(UUID uuid1, UUID uuid2) {
	return __builtin_memcmp(&uuid1, &uuid2, sizeof(UUID)) == 0;
}

static inline bool
uuidCompare(UUID uuid, const char *uuidstr) {
	UUID tmp{};

	return uuidCompare(uuid, tmp.fromString(uuidstr));
}