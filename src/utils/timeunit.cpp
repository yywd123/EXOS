#include <utils/timeunit.hpp>

//  抄的java(

__NAMESPACE_DECL(Utils::TimeUnit)

static inline uint64_t
x(uint64_t a, uint64_t b, uint64_t over) {
	if(a > over) return 0xffffffffffffffffull;
	return a * b;
}

uint64_t
convert(TimeUnit inputUnit, uint64_t time, TimeUnit outputUnit) {
	switch(inputUnit) {
	case DAYS: {
		switch(outputUnit) {
		case DAYS: return time;
		case HOURS: return x(time, 24, 384307168202282325);
		case MINUTE: return x(time, 1440, 6405119470038038);
		case SECONDS: return x(time, 86400, 106751991167300);
		case MILLISECONDS: return x(time, 86400000, 106751991167);
		case MICROSECONDS: return x(time, 86400000000, 106751991);
		case NANOSECONDS: return x(time, 86400000000000, 106751);
		}
	} break;
	case HOURS: {
		switch(outputUnit) {
		case DAYS: return time / 24;
		case HOURS: return time;
		case MINUTE: return x(time, 60, 153722867280912930);
		case SECONDS: return x(time, 3600, 2562047788015215);
		case MILLISECONDS: return x(time, 3600000, 2562047788015);
		case MICROSECONDS: return x(time, 3600000000, 2562047788);
		case NANOSECONDS: return x(time, 3600000000000, 2562047);
		}
	} break;
	case MINUTE: {
		switch(outputUnit) {
		case DAYS: return time / 1440;
		case HOURS: return time / 60;
		case MINUTE: return time;
		case SECONDS: return x(time, 60, 153722867280912930);
		case MILLISECONDS: return x(time, 60000, 153722867280912);
		case MICROSECONDS: return x(time, 60000000, 153722867280);
		case NANOSECONDS: return x(time, 60000000000, 153722867);
		}
	} break;
	case SECONDS: {
		switch(outputUnit) {
		case DAYS: return time / 86400;
		case HOURS: return time / 3600;
		case MINUTE: return time / 60;
		case SECONDS: return time;
		case MILLISECONDS: return x(time, 1000, 9223372036854775);
		case MICROSECONDS: return x(time, 1000000, 9223372036854);
		case NANOSECONDS: return x(time, 1000000000, 9223372036);
		}
	} break;
	case MILLISECONDS: {
		switch(outputUnit) {
		case DAYS: return time / 86400000;
		case HOURS: return time / 3600000;
		case MINUTE: return time / 60000;
		case SECONDS: return time / 1000;
		case MILLISECONDS: return time;
		case MICROSECONDS: return x(time, 1000, 9223372036854775);
		case NANOSECONDS: return x(time, 1000000, 9223372036854);
		}
	} break;
	case MICROSECONDS: {
		switch(outputUnit) {
		case DAYS: return time / 86400000000;
		case HOURS: return time / 3600000000;
		case MINUTE: return time / 60000000;
		case SECONDS: return time / 1000000;
		case MILLISECONDS: return time / 1000;
		case MICROSECONDS: return time;
		case NANOSECONDS: return x(time, 1000, 9223372036854775);
		}
	} break;
	case NANOSECONDS: {
		switch(outputUnit) {
		case DAYS: return time / 86400000000000;
		case HOURS: return time / 3600000000000;
		case MINUTE: return time / 60000000000;
		case SECONDS: return time / 1000000000;
		case MILLISECONDS: return time / 1000000;
		case MICROSECONDS: return time / 1000;
		case NANOSECONDS: return time;
		}
	}
	}

	return 0;
}

__NAMESPACE_END