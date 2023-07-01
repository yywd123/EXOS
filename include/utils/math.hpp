#pragma once

#define MATH_PI 3.1415926

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) < 0 ? -(x) : (x))

float
floor(float x);

float
ceil(float x);

float
round(float x);

float
sqrt(float x);

float
pow(float base, int32_t exp);

float
fmod(float x, float y);

float
atan(float x);

float
atan2(float x, float y);

float
cos(float x);

float
acos(float x);

float
sin(float x);