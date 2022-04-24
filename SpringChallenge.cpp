// ..\Codin\Vector.cpp
#include "Vector.h"

#include <iostream>

std::istream& operator>>(std::istream& in, Vector& vec)
{
	in >> vec.x >> vec.y >> vec.z;
	return in;
}

std::ostream& operator<<(std::ostream& out, const Vector& vec)
{
	out << vec.x << ' ' << vec.y << ' ' << vec.z;
	return out;
}
// Main.cpp
int main()
{
	return 0;
}
