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
