#pragma once

#include <iosfwd>

struct Vector
{
	int x, y;
};

std::istream& operator>>(std::istream& in, Vector& vec);
std::ostream& operator<<(std::ostream& out, const Vector& vec);
