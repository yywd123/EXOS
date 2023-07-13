#pragma once

__NAMESPACE_DECL(String)

static inline const char *
toLowerCase(const char *str) {
	char *s = (char *)str;
	__iter(length(str)) {
		if(str[i] >= 'A' && str[i] <= 'Z') s[i] += 32;
	}

	return str;
}

static inline index_t
indexOf(const char *str, char c) {
	__iter(length(str)) {
		if(str[i] == c) return i;
	}

	return 0;
}

static inline uint64_t
parseHex(const char *str, size_t len) {
	uint64_t result = 0;
	const char *hexDigits = "0123456789abcdef";

	__iter(len) {
		if(str[i] == 0) break;
		result *= 16;
		result += indexOf(hexDigits, str[i]);
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

__NAMESPACE_END