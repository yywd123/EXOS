#pragma once

#include <utils/math.hpp>
#include <vector>

__NAMESPACE_DECL(String)

static inline const char *
toLowerCase(const char *str) {
	char *s = (char *)str;
	_iter(length(str)) {
		if(str[i] >= 'A' && str[i] <= 'Z') s[i] += 32;
	}

	return str;
}

static inline index_t
indexOf(const char *str, char c) {
	_iter(length(str)) {
		if(str[i] == c) return i;
	}

	return -1;
}

static inline uint64_t
parseHex(const char *str, size_t len) {
	uint64_t result = 0;
	const char *hexDigits = "0123456789abcdef";

	_iter(len) {
		index_t j = 0;
		if(str[i] == 0 || (j = indexOf(hexDigits, str[i])) == -1) break;
		result *= 16;
		result += j;
	}

	return result;
}

static inline uint8_t
parseHexByte(const char *str) {
	return parseHex(str, 2);
}

static inline uint16_t
parseHexShort(const char *str) {
	return parseHex(str, 4);
}

static inline uint32_t
parseHexInt(const char *str) {
	return parseHex(str, 8);
}

static inline uint64_t
parseHexLong(const char *str) {
	return parseHex(str, 16);
}

static inline int64_t
parseInt(const char *str) {
	int64_t result = 0;
	size_t len = length(str);
	bool isNegative = false;

	if(str[0] == '-') {
		isNegative = true;
		str++;
		len--;
	}

	for(int i = 0; i < len; i++) {
		if(str[i] < '0' || str[i] > '9') break;
		result *= 10;
		result += str[i] - '0';
	}

	return isNegative ? (-result) : result;
}

static inline bool
startsWith(const char *str, const char *prefix) {
	const char *s = str;
	while(*s && *s == *prefix) {
		++s;
		++prefix;
	}

	return *prefix == 0;
}

static inline bool
compare(const char *s1, const char *s2) {
	while(*s1 != 0 && *s1++ == *s2++)
		;
	return *s1 == 0 && *s2 == 0;
}

__NAMESPACE_END