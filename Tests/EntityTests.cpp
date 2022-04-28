#include "pch.h"

#include "../Codin/Entity.h"
#include "../Codin/EntityDescription.h"

TEST(EntityTest, CreateEmpty)
{
	const Entity entity({}, 0);
	EXPECT_EQ(entity.GetPosition(), Vector());
}

TEST(EntityTest, CreateFromDescription)
{
	const Entity entity({ 11, 0, { 11202, 8350 }, 5, 0, 10, { 341, -207 }, 1, 2 }, 0);
	EXPECT_EQ(entity.GetId(), 11);
	EXPECT_EQ(entity.GetType(), EntityType::Monster);
	EXPECT_EQ(entity.GetPosition(), Vector({ 11202, 8350 }));
	EXPECT_EQ(entity.GetVelocity(), Vector({ 341, -207 }));
	EXPECT_EQ(entity.GetShieldLife(), 5);
	EXPECT_EQ(entity.IsControlled(), false);
	EXPECT_EQ(entity.GetHealt(), 10);
	EXPECT_EQ(entity.IsNearBase(), true);
	EXPECT_EQ(entity.GetThreatFor(), ThreatFor::OpponentsBase);
}

TEST(EntityTest, CreateMyHeroFromDescription)
{
	const Entity entity({ 1, 1, { 2186, 1062 }, 0, 0, -1, { -1, -1 }, -1, -1 }, 0);
	EXPECT_EQ(entity.GetId(), 1);
	EXPECT_EQ(entity.GetType(), EntityType::MyHero);
	EXPECT_EQ(entity.GetPosition(), Vector({ 2186, 1062 }));
	EXPECT_EQ(entity.GetVelocity(), Vector({ -1, -1 }));
	EXPECT_EQ(entity.GetShieldLife(), 0);
	EXPECT_EQ(entity.IsControlled(), false);
	EXPECT_EQ(entity.GetHealt(), -1);
	EXPECT_EQ(entity.IsNearBase(), false);
	EXPECT_EQ(entity.GetThreatFor(), ThreatFor::None);
}
