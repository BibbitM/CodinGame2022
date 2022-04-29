#include "pch.h"

#include "../Codin/Math.h"

TEST(MathTest, Pow2)
{
	EXPECT_EQ(Pow2(0), 0);
	EXPECT_EQ(Pow2(1), 1);
	EXPECT_EQ(Pow2(2), 4);
	EXPECT_EQ(Pow2(10), 100);
	EXPECT_EQ(Pow2(-1), 1);
	EXPECT_EQ(Pow2(-3), 9);
}
