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

TEST(VectorTest, Nagate)
{
	EXPECT_EQ(-Vector({ 0, 0 }), Vector({ 0, 0 }));
	EXPECT_EQ(-Vector({ 1, 1 }), Vector({ -1, -1 }));
	EXPECT_EQ(-Vector({ -3, -5 }), Vector({ 3, 5 }));
}

TEST(VectorTest, Distance2)
{
	EXPECT_EQ(Distance2(Vector({ 0, 0 }), Vector({ 0, 0 })), 0);
	EXPECT_EQ(Distance2(Vector({ 11, 230 }), Vector({ 11, 230 })), 0);
	EXPECT_EQ(Distance2(Vector({ 100, 0 }), Vector({ 200, 0 })), 10000);
	EXPECT_EQ(Distance2(Vector({ 0, 1 }), Vector({ 0, 2 })), 1 );
	EXPECT_EQ(Distance2(Vector({ 3, 4 }), Vector({ 5, 6 })), 8);
}

TEST(VectorTest, Length2)
{
	EXPECT_EQ(Vector({ 0, 0 }).Length2(), 0);
	EXPECT_EQ(Vector({ 1, 0 }).Length2(), 1);
	EXPECT_EQ(Vector({ 0, -1 }).Length2(), 1);
	EXPECT_EQ(Vector({ -3, 0 }).Length2(), 9);
	EXPECT_EQ(Vector({ 0, 2 }).Length2(), 4);
	EXPECT_EQ(Vector({ 3, 4 }).Length2(), 25);
	EXPECT_EQ(Vector({ -10, -10 }).Length2(), 200);
}

TEST(VectorTest, Lengthed)
{
	EXPECT_EQ(Vector({ 1, 0 }).Lengthed(1), Vector({ 1, 0 }));
	EXPECT_EQ(Vector({ 0, -1 }).Lengthed(1), Vector({ 0, -1 }));
	EXPECT_EQ(Vector({ 0, 0 }).Lengthed(100), Vector({ 0, 0 }));
	EXPECT_EQ(Vector({ 0, 1 }).Lengthed(100), Vector({ 0, 100 }));
	EXPECT_EQ(Vector({ -1, 0 }).Lengthed(100), Vector({ -100, 0 }));
	EXPECT_EQ(Vector({ 10, -10 }).Lengthed(0), Vector({ 0, 0 }));

	EXPECT_EQ(Vector({ 10, 0 }).Lengthed(10), Vector({ 10, 0 }));
	EXPECT_EQ(Vector({ 0, -5 }).Lengthed(2), Vector({ 0, -2 }));
	EXPECT_EQ(Vector({ 3, 4 }).Lengthed(5), Vector({ 3, 4 }));
	EXPECT_EQ(Vector({ 4, -3 }).Lengthed(10), Vector({ 8, -6 }));

	EXPECT_EQ(Vector({ 1, 1 }).Lengthed(10), Vector({ 7, 7 }));
}
