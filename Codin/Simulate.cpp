#include "Simulate.h"

#include "Entity.h"
#include "Rules.h"

Vector Simulate::GetNearestBasePosition(const Entity& entity)
{
	return entity.GetPosition().x < Rules::mapSize.x / 2 ? Vector{ 0, 0 } : Rules::mapSize;;
}

void Simulate::Update(Entity& entity)
{
	entity.SetPosition(entity.GetTargetPosition());

	const Vector basePosition = GetNearestBasePosition(entity);
	if (Distance2(entity.GetPosition(), basePosition) <= Pow2(Rules::monsterBaseAttackRange))
	{
		entity.SetNearBase(true);
		entity.SetVelocity((basePosition - entity.GetPosition()).Lengthed(Rules::monsterMoveRange));
	}
}

int Simulate::EnemyFramesToDealDamage(const Entity& entity)
{
	if (entity.GetThreatFor() == ThreatFor::None)
		return std::numeric_limits<int>::max();

	const Vector basePosition = GetNearestBasePosition(entity);

	constexpr int maxFramesToDealDamage2 = static_cast<int>(static_cast<float>(Rules::mapSize.Length2()) / static_cast<float>(Pow2(Rules::monsterMoveRange)));
	int framesToDealDamge = 0;
	Entity simulatedEntity{ entity };
	while (Distance2(simulatedEntity.GetTargetPosition(), basePosition) > Pow2(Rules::monsterBaseDestroyRange)
		&& Pow2(framesToDealDamge) < maxFramesToDealDamage2)
	{
		Update(simulatedEntity);
		++framesToDealDamge;
	}

	return framesToDealDamge;
}
