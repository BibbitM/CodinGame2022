#include "pch.h"

#include "../Codin/Vector.h"

#include <sstream>

TEST(VectorTest, CreateEmpty)
{
	Vector vec{};
	EXPECT_EQ(vec.x, 0);
	EXPECT_EQ(vec.y, 0);
}

TEST(VectorTest, CreateXYZ)
{
	Vector vec{ 1, 2 };
	EXPECT_EQ(vec.x, 1);
	EXPECT_EQ(vec.y, 2);
}

TEST(VectorTest, ReadFromInput)
{
	std::istringstream in{ "10 20" };
	Vector vec;
	in >> vec;
	EXPECT_EQ(vec.x, 10);
	EXPECT_EQ(vec.y, 20);
}

TEST(VectorTest, WriteToInput)
{
	Vector vec{ 1, 10 };
	std::ostringstream out;
	out << vec;
	EXPECT_EQ(out.str(), "1 10");
}

TEST(VectorTest, Addition)
{
	Vector a{ 1, 2 };
	Vector b{ 10, 200 };
	Vector add{ 11, 202 };
	EXPECT_EQ(a + b, add);
}

TEST(VectorTest, AdditionInPlace)
{
	Vector a{ 3, 4 };
	Vector b{ 1, 2 };
	Vector add = a + b;
	a += b;
	EXPECT_EQ(a, add);
}

TEST(VectorTest, Subtraction)
{
	Vector a{ 100, 10 };
	Vector b{ 200, -10 };
	Vector sub{ -100, 20 };
	EXPECT_EQ(a - b, sub);
}

TEST(VectorTest, SubtractionInPlace)
{
	Vector a{ 1, 4 };
	Vector b{ 2, 3 };
	Vector sub = a - b;
	a -= b;
	EXPECT_EQ(a, sub);
}

TEST(VectorTest, Multiplication)
{
	Vector a{ 1, 2 };
	int b = 1000;
	Vector mul{ 1000, 2000 };
	EXPECT_EQ(a * b, mul);
}

TEST(VectorTest, MultiplicationInPlace)
{
	Vector a{ -1, 3 };
	int b = 10;
	Vector mul = a * b;
	a *= b;
	EXPECT_EQ(a, mul);
}

TEST(VectorTest, Division)
{
	Vector a{ 55, 12 };
	int b = 10;
	Vector div{ 5, 1 };
	EXPECT_EQ(a / b, div);
}

TEST(VectorTest, DivisionInPlace)
{
	Vector a{ 900, 99 };
	int b = 100;
	Vector div = a / b;
	a /= b;
	EXPECT_EQ(a, div);
}
