#include "Vector.h"

#include <iostream>

std::istream& operator>>(std::istream& in, Vector& vec)
{
	in >> vec.x >> vec.y;
	return in;
}

std::ostream& operator<<(std::ostream& out, const Vector& vec)
{
	out << vec.x << ' ' << vec.y;
	return out;
}

Vector Vector::Lengthed(int length) const
{
	if (!x && !y)
		return {};

	float flen = std::sqrt(static_cast<float>(Length2()));
	float fx = static_cast<float>(x) / flen * static_cast<float>(length);
	float fy = static_cast<float>(y) / flen * static_cast<float>(length);

	return { static_cast<int>(fx), static_cast<int>(fy) };
}

Vector Vector::Limited(int lenght) const
{
	return (Length2() <= Pow2(lenght)) ? *this : Lengthed(lenght);
}

Vector Vector::Perpendicular() const
{
	return { -y, x };
}
