#pragma once

#include <cassert>

#include "integers.h"

//2d vector
template<typename T>
class V2
{
public:
	inline V2() = default;

	inline V2(T inX, T inY)
		: x(inX)
		, y(inY)
	{}

	inline T& operator[](size_t index)
	{
		assert(index >= 0 && index < 2);
		return index == 0 ? x : y;
	}

	inline const T& operator[](size_t index) const
	{
		assert(index >= 0 && index < 2);
		return index == 0 ? x : y;
	}

	inline V2& operator+=(const V2& another)
	{
		x += another.x;
		y += another.y;
		return *this;
	}

	inline V2 operator+(const V2& another) const
	{
		V2 copy = *this;
		copy += another;
		return copy;
	}

	inline V2& operator-=(const V2& another)
	{
		x -= another.x;
		y -= another.y;
		return *this;
	}

	inline V2 operator-(const V2& another) const
	{
		V2 copy = *this;
		copy -= another;
		return copy;
	}

	inline bool operator==(const V2& another) const
	{
		return x == another.x && y == another.y;
	}

	inline bool operator>(const V2& v) const
	{
		return x > v.x && y > v.y;
	}

	inline bool operator>=(const V2& v) const
	{
		return x >= v.x && y >= v.y;
	}

	inline bool operator<(const V2& v) const
	{
		return x < v.x && y < v.y;
	}

	inline bool operator<=(const V2& v) const
	{
		return x <= v.x && y <= v.y;
	}

	inline static V2 Zero()
	{
		return V2(0, 0);
	}

	T x;
	T y;
};

using V2f = V2<float>;
using V2i = V2<i32>;
