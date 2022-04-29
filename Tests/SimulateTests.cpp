#include "pch.h"

#include "../Codin/Entity.h"
#include "../Codin/EntityDescription.h"
#include "../Codin/Rules.h"
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

TEST(SimulateTest, PositionAfterFrames)
{
	Entity entity({ 157, 0, { 1987, 4969 }, 0, 0, 28, { -202, -345 }, 0, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 0), Vector({ 1987, 4969 }));
	//Entity entity({ 157, 0, { 1785, 4624 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 1), Vector({ 1785, 4624 }));
	//Entity entity({ 157, 0, { 1641, 4251 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 2), Vector({ 1641, 4251 }));
	//Entity entity({ 157, 0, { 1497, 3878 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 3), Vector({ 1497, 3878 }));
	//Entity entity({ 157, 0, { 1353, 3505 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 4), Vector({ 1353, 3505 }));
	//Entity entity({ 157, 0, { 1209, 3132 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 5), Vector({ 1209, 3132 }));
	//Entity entity({ 157, 0, { 1065, 2759 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 6), Vector({ 1065, 2759 }));
	//Entity entity({ 157, 0, { 921, 2386 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 7), Vector({ 921, 2386 }));
	//Entity entity({ 157, 0, { 777, 2013 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 8), Vector({ 777, 2013 }));
	//Entity entity({ 157, 0, { 633, 1640 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 9), Vector({ 633, 1640 }));
	//Entity entity({ 157, 0, { 489, 1267 }, 0, 0, 28, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 10), Vector({ 489, 1267 }));
	//Entity entity({ 157, 0, { 345, 894 }, 0, 0, 24, { -144, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 11), Vector({ 345, 894 }));
	//Entity entity({ 157, 0, { 201, 521 }, 0, 0, 18, { -143, -373 }, 1, 1 }, 0);
	EXPECT_EQ(Simulate::PositionAfterFrames(entity, 12), Vector({ 201, 521 }));
}

TEST(SimulateTest, EnemyFramesToAttackBaseNoThreat)
{
	const Entity entity({ 9, 0, { 2907, 8353 }, 0, 0, 10, { -318, -241 }, 0, 0 }, 0);

	EXPECT_EQ(Simulate::EnemyFramesToAttackBase(entity), std::numeric_limits<int>::max());
}

TEST(SimulateTest, EnemyFramesToAttackBaseNoThreatFarCorner)
{
	const Entity entity({ 12, 0, { 15686, 1361 }, 0, 0, 8, { 319, 240 }, 0, 0 }, 0);

	EXPECT_EQ(Simulate::EnemyFramesToAttackBase(entity), std::numeric_limits<int>::max());
}

TEST(SimulateTest, EnemyFramesToAttackBaseAtBase)
{
	const Entity entity({ 157, 0, { 201, 521 }, 0, 0, 18, { -143, -373 }, 1, 1 }, 0);

	EXPECT_EQ(Simulate::EnemyFramesToAttackBase(entity), 0);
}

TEST(SimulateTest, EnemyFramesToAttackBaseAtBaseFarCorner)
{
	const Entity entity({ 99, 0, { 17087, 8882 }, 0, 0, 5, { 390, 84 }, 1, 1 }, 0);

	EXPECT_EQ(Simulate::EnemyFramesToAttackBase(entity), 0);
}

TEST(SimulateTest, EnemyFramesToAttackBaseOneFrame)
{
	const Entity entity({ 157, 0, { 345, 894 }, 0, 0, 24, { -144, -373 }, 1, 1 }, 0);

	EXPECT_EQ(Simulate::EnemyFramesToAttackBase(entity), 1);
}

TEST(SimulateTest, EnemyFramesToAttackBaseOneFrameFarCorner)
{
	const Entity entity({ 99, 0, { 16697, 8797 }, 0, 0, 7, { 390, 85 }, 1, 1 }, 0);

	EXPECT_EQ(Simulate::EnemyFramesToAttackBase(entity), 1);
}

TEST(SimulateTest, EnemyFramesToAttackBaseMultipleFrames)
{
	const Entity entity({ 157, 0, { 2593, 6004 }, 0, 0, 28, { -202, -345 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::EnemyFramesToAttackBase(entity), 15);
}

TEST(SimulateTest, EnemyFramesToAttackBaseMultipleFramesFarCorner)
{
	const Entity entity({ 99, 0, { 12435, 8119 }, 0, 0, 21, { 362, -168 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::EnemyFramesToAttackBase(entity), 12);
}

TEST(SimulateTest, EnemyFramesToAttackBaseInvalidInput)
{
	const Entity entity({ 9, 0, { 8000, 8353 }, 0, 0, 10, { 0, 0 }, 0, 1 }, 0);

	EXPECT_NE(Simulate::EnemyFramesToAttackBase(entity), std::numeric_limits<int>::max());
}

TEST(SimulateTest, HeroFramesToAttackEnemyInAttackRange)
{
	const Entity hero({ 1, 1, { 6001, 1 }, 0, 0, -1, { -1, -1 }, -1, -1 }, 0);
	const Entity enemy({ 10, 0, { 6000, 0 }, 0, 0, 1, { -400, 0 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::HeroFramesToAttackEnemy(hero, enemy), 0);
}

TEST(SimulateTest, HeroFramesToAttackEnemyInMoveRange)
{
	const Entity hero({ 1, 1, { 7000, 1 }, 0, 0, -1, { -1, -1 }, -1, -1 }, 0);
	const Entity enemy({ 10, 0, { 6000, 0 }, 0, 0, 1, { -400, 0 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::HeroFramesToAttackEnemy(hero, enemy), 0);
}

TEST(SimulateTest, HeroFramesToAttackEnemyAfter1Move)
{
	const Entity hero({ 1, 1, { 7700, 1 }, 0, 0, -1, { -1, -1 }, -1, -1 }, 0);
	const Entity enemy({ 10, 0, { 6000, 0 }, 0, 0, 1, { -400, 0 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::HeroFramesToAttackEnemy(hero, enemy), 1);
}

TEST(SimulateTest, HeroFramesToAttackEnemyAfter3Moves)
{
	const Entity hero({ 1, 1, { 9200, 1 }, 0, 0, -1, { -1, -1 }, -1, -1 }, 0);
	const Entity enemy({ 10, 0, { 6000, 0 }, 0, 0, 1, { -400, 0 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::HeroFramesToAttackEnemy(hero, enemy), 3);
}

TEST(SimulateTest, HeroFramesToCastSpeellAtEnemy)
{
	const Entity hero({ 1, 1, { 6001, 1 }, 0, 0, -1, { -1, -1 }, -1, -1 }, 0);
	const Entity enemy({ 10, 0, { 6000, 0 }, 0, 0, 1, { -400, 0 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::HeroFramesToCastSpell(hero, enemy, Rules::spellWindRange), 0);
	EXPECT_EQ(Simulate::HeroFramesToCastSpell(hero, enemy, Rules::spellControlRange), 0);
}

TEST(SimulateTest, HeroFramesToCastSpeellAtEnemyAfter1Move)
{
	const Entity hero({ 1, 1, { 7700, 1 }, 0, 0, -1, { -1, -1 }, -1, -1 }, 0);
	const Entity enemy({ 10, 0, { 6000, 0 }, 0, 0, 1, { -400, 0 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::HeroFramesToCastSpell(hero, enemy, Rules::spellWindRange), 1);
	EXPECT_EQ(Simulate::HeroFramesToCastSpell(hero, enemy, Rules::spellControlRange), 0);
}

TEST(SimulateTest, HeroFramesToCastSpeellAtEnemyAfterAFewMoves)
{
	const Entity hero({ 1, 1, { 9200, 1 }, 0, 0, -1, { -1, -1 }, -1, -1 }, 0);
	const Entity enemy({ 10, 0, { 6000, 0 }, 0, 0, 1, { -400, 0 }, 0, 1 }, 0);

	EXPECT_EQ(Simulate::HeroFramesToCastSpell(hero, enemy, Rules::spellWindRange), 3);
	EXPECT_EQ(Simulate::HeroFramesToCastSpell(hero, enemy, Rules::spellControlRange), 2);
}

TEST(SimulateTest, FramesToKill)
{
	EXPECT_EQ(Simulate::FramesToKill(0), 0);
	EXPECT_EQ(Simulate::FramesToKill(1), 0);
	EXPECT_EQ(Simulate::FramesToKill(2), 1);
	EXPECT_EQ(Simulate::FramesToKill(4), 2);
	EXPECT_EQ(Simulate::FramesToKill(8), 4);
	EXPECT_EQ(Simulate::FramesToKill(9), 4);
	EXPECT_EQ(Simulate::FramesToKill(11), 5);
}
