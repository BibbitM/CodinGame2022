#pragma once

#include <cmath>

constexpr inline int Pow2(int val)
{
	return val * val;
}

inline int Sqrt(int val)
{
	return static_cast<int>(std::sqrt(static_cast<float>(val)));
}
