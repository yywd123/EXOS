#pragma once

__NAMESPACE_DECL(Drivers::CMOS)

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t dayOfWeek;
	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t century;
} Time;

Time
getTime();

void
setTimeOffset(int8_t hour, uint8_t minute);

void __INIT
initialize();

static inline bool
compareTime(Time time1, Time time2) {
	return __builtin_memcmp(&time1, &time2, sizeof(Time)) == 0;
}

__NAMESPACE_END