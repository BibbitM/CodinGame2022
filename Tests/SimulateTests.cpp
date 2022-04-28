#include "pch.h"

#include "../Codin/Entity.h"
#include "../Codin/EntityDescription.h"
#include "../Codin/Simulate.h"

TEST(SimulateTest, Update)
{
	Entity entity({ 13, 0, { 1005, 8609 }, 0, 0, 10, { -381, -119 }, 0, 0 }, 0);

	Simulate::Update(entity);

	//Entity entity({ 13, 0, { 624, 8490 }, 0, 0, 10, { -381, -119 }, 0, 0 }, 0);
	EXPECT_EQ(entity.GetPosition(), Vector({ 624, 8490 }));
	EXPECT_EQ(entity.GetVelocity(), Vector({ -381, -119 }));
}

TEST(SimulateTest, UpdateToNearBase)
{
	Entity entity({ 155, 0, { 4547, 2121 }, 0, 0, 28, { -194, -349 }, 0, 1 }, 0);

	Simulate::Update(entity);

	//Entity entity({ 155, 0, { 4353, 1772 }, 0, 0, 28, { -370, -150 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetPosition(), Vector({ 4353, 1772 }));
	EXPECT_EQ(entity.GetVelocity(), Vector({ -370, -150 }));
	EXPECT_EQ(entity.IsNearBase(), true);
}

TEST(SimulateTest, UpdateToNearBaseFarCorner)
{
	Entity entity({ 146, 0, { 16141, 4041 }, 0, 0, 21, { 333, 220 }, 0, 1 }, 0);

	Simulate::Update(entity);

	//Entity entity({ 146, 0, { 16474, 4261 }, 0, 0, 19, { 94, 388 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetPosition(), Vector({ 16474, 4261 }));
	EXPECT_EQ(entity.GetVelocity(), Vector({ 94, 388 }));
	EXPECT_EQ(entity.IsNearBase(), true);

}

TEST(SimulateTest, UpdateCalculateVelocityToReachBase)
{
	Entity entity({ 157, 0, { 1987, 4969 }, 0, 0, 28, { -202, -345 }, 0, 1 }, 0);

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 1785, 4624 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 1641, 4251 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 1497, 3878 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 1353, 3505 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 1209, 3132 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 1065, 2759 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 921, 2386 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 777, 2013 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 633, 1640 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 489, 1267 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 345, 894 }, 0, 0, 24, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -144, -373 }));

	Simulate::Update(entity);
	//Entity entity({ 157, 0, { 201, 521 }, 0, 0, 18, { -143, -373 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ -143, -373 }));
}

TEST(SimulateTest, UpdateCalculateVelocityToReachBaseFarCorner)
{
	Entity entity({ 99, 0, { 12435, 8119 }, 0, 0, 21, { 362, -168 }, 0, 1 }, 0);

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 12797, 7951 }, 0, 0, 21, { 390, 84 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 84 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 13187, 8035 }, 0, 0, 21, { 390, 84 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 84 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 13577, 8119 }, 0, 0, 21, { 390, 84 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 84 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 13967, 8203 }, 0, 0, 21, { 390, 85 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 85 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 14357, 8288 }, 0, 0, 19, { 390, 85 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 85 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 14747, 8373 }, 0, 0, 17, { 390, 85 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 85 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 15137, 8458 }, 0, 0, 15, { 390, 84 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 84 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 15527, 8542 }, 0, 0, 13, { 390, 85 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 85 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 15917, 8627 }, 0, 0, 11, { 390, 85 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 85 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 16307, 8712 }, 0, 0, 9, { 390, 85 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 85 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 16697, 8797 }, 0, 0, 7, { 390, 85 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 85 }));

	Simulate::Update(entity);
	//Entity entity({ 99, 0, { 17087, 8882 }, 0, 0, 5, { 390, 84 }, 1, 1 }, 0);
	EXPECT_EQ(entity.GetVelocity(), Vector({ 390, 84 }));
}

TEST(SimulateTest, FramesToDealDamageNoThreat)
{
	const Entity entity({ 9, 0, { 2907, 8353 }, 0, 0, 10, { -318, -241 }, 0, 0 }, 0);

	EXPECT_EQ(Simulate::FramesToDealDamage(entity), std::numeric_limits<int>::max());
}

TEST(SimulateTest, FramesToDealDamageNoThreatFarCorner)
{
	const Entity entity({ 12, 0, { 15686, 1361 }, 0, 0, 8, { 319, 240 }, 0, 0 }, 0);

	EXPECT_EQ(Simulate::FramesToDealDamage(entity), std::numeric_limits<int>::max());
}

TEST(SimulateTest, FramesToDealDamageAtBase)
{
	const Entity entity({ 157, 0, { 201, 521 }, 0, 0, 18, { -143, -373 }, 1, 1 }, 0);

	EXPECT_EQ(Simulate::FramesToDealDamage(entity), 0);
}

TEST(SimulateTest, FramesToDealDamageAtBaseFarCorner)
{
	const Entity entity({ 99, 0, { 17087, 8882 }, 0, 0, 5, { 390, 84 }, 1, 1 }, 0);

	EXPECT_EQ(Simulate::FramesToDealDamage(entity), 0);
}

TEST(SimulateTest, FramesToDealDamageOneFrame)
{
	const Entity entity({ 157, 0, { 345, 894 }, 0, 0, 24, { -144, -373 }, 1, 1 }, 0);

	EXPECT_EQ(Simulate::FramesToDealDamage(entity), 1);
}

TEST(SimulateTest, FramesToDealDamageOneFrameFarCorner)
{
	const Entity entity({ 99, 0, { 16697, 8797 }, 0, 0, 7, { 390, 85 }, 1, 1 }, 0);

	EXPECT_EQ(Simulate::FramesToDealDamage(entity), 1);
}

TEST(SimulateTest, FramesToDealDamageMultipleFrames)
{
	const Entity entity({ 157, 0, { 2593, 6004 }, 0, 0, 28, { -202, -345 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::FramesToDealDamage(entity), 15);
}

TEST(SimulateTest, FramesToDealDamageMultipleFramesFarCorner)
{
	const Entity entity({ 99, 0, { 12435, 8119 }, 0, 0, 21, { 362, -168 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::FramesToDealDamage(entity), 12);
}

TEST(SimulateTest, FramesToDealDamageInvalidInput)
{
	Entity entity({ 9, 0, { 2907, 8353 }, 0, 0, 10, { -318, -241 }, 0, 1 }, 0);

	EXPECT_NE(Simulate::FramesToDealDamage(entity), std::numeric_limits<int>::max());
}
