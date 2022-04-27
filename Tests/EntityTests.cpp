#include "pch.h"

#include "../Codin/Entity.h"
#include "../Codin/EntityDescription.h"

TEST(EntityTest, CreateEmpty)
{
	Entity entity({}, 0);
	EXPECT_EQ(entity.GetPosition(), Vector());
}
