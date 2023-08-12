#pragma once

#include <utils/math.hpp>

typedef uint32_t RGBColor;

__NAMESPACE_DECL(Display)

template<typename T>
class _Vec2D {
public:
	T x, y;

	_Vec2D<T> &
	operator+=(const _Vec2D &v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	_Vec2D<T>
	operator+(const _Vec2D &v) {
		_Vec2D v1 = *this;
		v1 += v;
		return v1;
	}

	_Vec2D<T> &
	operator-=(const _Vec2D &v) {
		if(x < v.x)
			x = 0;
		else
			x -= v.x;

		if(y < v.y)
			y = 0;
		else
			y -= v.y;
		return *this;
	}

	_Vec2D<T>
	operator-(const _Vec2D &v) {
		_Vec2D v1 = *this;
		v1 -= v;
		return v1;
	}
};

template<typename T>
class _BoundingBox {
public:
	T x1, y1, x2, y2;

	_BoundingBox<T>
			*
			subBoundingBox(uint32_t topX, uint32_t topY, uint32_t bottomX, uint32_t bottomY) {
		uint32_t minX = __min(x1, x2);
		uint32_t minY = __min(y1, y2);
		uint32_t maxX = __max(x1, x2);
		uint32_t maxY = __max(y1, y2);
		if(topX + bottomX > maxX || topY + bottomY > maxY) return new _BoundingBox<T>{0, 0, 0, 0};
		return new _BoundingBox<T>{minX + topX, minY + topY, maxX - bottomX, maxY - bottomY};
	}
};

using Vec2D = _Vec2D<uint32_t>;
using BoundingBox = _BoundingBox<uint32_t>;

__NAMESPACE_END