#include "Simulate.h"

#include "Entity.h"
#include "Rules.h"

void Simulate::Update(Entity& entity)
{
	entity.SetPosition(entity.GetTargetPosition());

	const Vector basePosition = entity.GetPosition().x < Rules::mapSize.x / 2 ? Vector{ 0, 0 } : Rules::mapSize;
	if (DistanceSqr(entity.GetPosition(), basePosition) <= Sqr(Rules::monsterBaseAttackRange))
	{
		entity.SetNearBase(true);
		entity.SetVelocity((basePosition - entity.GetPosition()).Lengthed(Rules::monsterMoveRange));
	}
}
