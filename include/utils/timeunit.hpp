#pragma once

__NAMESPACE_DECL(Utils::TimeUnit)

typedef enum {
	DAYS,
	HOURS,
	MINUTE,
	SECONDS,
	MILLISECONDS,
	MICROSECONDS,
	NANOSECONDS
} TimeUnit;

uint64_t
convert(TimeUnit inputUnit, uint64_t time, TimeUnit outputUnit);

__NAMESPACE_END