// ..\Codin\Vector.cpp
// #include "Vector.h" begin
// ..\Codin\Vector.h
// #pragma once
#include <iosfwd>

struct Vector
{
	int x, y;
};

std::istream& operator>>(std::istream& in, Vector& vec);
std::ostream& operator<<(std::ostream& out, const Vector& vec);
// #include "Vector.h" end

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
// Main.cpp
int main()
{
	return 0;
}
