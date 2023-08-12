#include <utils/math.hpp>

#define __Accuracy 0.0000001f

float
floor(float x) {
	return (x - 1 + __Accuracy) / 1;
}

float
ceil(float x) {
	return (x + 1 - __Accuracy) / 1;
}

float
round(float x) {
	return (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}

float
sqrt(float x) {
	float result;
	ASM("fsqrt"
			: "=t"(result)
			: "0"(x));
	return result;
}

float
fmod(float x, float y) {
	return x - (int32_t)(x / y) * y;
}

float
atan(float x) {
	// https://stackoverflow.com/questions/11930594/calculate-atan2-without-std-functions-or-c99
	static const uint32_t sign_mask = 0x80000000;
	static const float b = 0.596227f;

	// Extract the sign bit
	uint32_t ux_s = sign_mask & (uint32_t &)x;

	// Calculate the arctangent in the first quadrant
	float bx_a = __abs(b * x);
	float num = bx_a + x * x;
	float atan_1q = num / (1.f + bx_a + num);

	// Restore the sign bit
	uint32_t atan_2q = ux_s | (uint32_t &)atan_1q;
	return (float &)atan_2q;
}

float
atan2(float x, float y) {
	// https://stackoverflow.com/questions/11930594/calculate-atan2-without-std-functions-or-c99
	static const uint32_t sign_mask = 0x80000000;
	static const float b = 0.596227f;

	// Extract the sign bits
	uint32_t ux_s = sign_mask & (uint32_t &)x;
	uint32_t uy_s = sign_mask & (uint32_t &)y;

	// Determine the quadrant offset
	float q = (float)((~ux_s & uy_s) >> 29 | ux_s >> 30);

	// Calculate the arctangent in the first quadrant
	float bxy_a = __abs(b * x * y);
	float num = bxy_a + y * y;
	float atan_1q = num / (x * x + bxy_a + num);

	// Translate it to the proper quadrant
	uint32_t uatan_2q = (ux_s ^ uy_s) | (uint32_t &)atan_1q;
	return q + (float &)uatan_2q;
}

float
cos(float x) {
	float result;
	ASM("fcos"
			: "=t"(result)
			: "0"(x));
	return result;
}

float
acos(float x) {
	return atan2(sqrt(1.0 - x * x), x);
}

float
sin(float x) {
	float result;
	ASM("fsin"
			: "=t"(result)
			: "0"(x));
	return result;
}