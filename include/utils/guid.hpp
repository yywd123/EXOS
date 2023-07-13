#pragma once

#include <utils/uuid.hpp>

typedef UUID GUID;

static inline bool
guidCompare(GUID guid1, GUID guid2) {
	return uuidCompare(guid1, guid2);
}

static inline bool
guidCompare(GUID guid, const char *guidstr) {
	return uuidCompare(guid, guidstr);
}