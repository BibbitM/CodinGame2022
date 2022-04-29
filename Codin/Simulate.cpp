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
	if (DistanceSqr(entity.GetPosition(), basePosition) <= Sqr(Rules::monsterBaseAttackRange))
	{
		entity.SetNearBase(true);
		entity.SetVelocity((basePosition - entity.GetPosition()).Lengthed(Rules::monsterMoveRange));
	}
}

int Simulate::FramesToDealDamage(const Entity& entity)
{
	if (entity.GetThreatFor() == ThreatFor::None)
		return std::numeric_limits<int>::max();

	const Vector basePosition = GetNearestBasePosition(entity);

	constexpr int maxFramesToDealDamageSqr = static_cast<int>(static_cast<float>(Rules::mapSize.LengthSqr()) / static_cast<float>(Sqr(Rules::monsterMoveRange)));
	int framesToDealDamge = 0;
	Entity simulatedEntity{ entity };
	while (DistanceSqr(simulatedEntity.GetTargetPosition(), basePosition) > Sqr(Rules::monsterBaseDestroyRange)
		&& Sqr(framesToDealDamge) < maxFramesToDealDamageSqr)
	{
		Update(simulatedEntity);
		++framesToDealDamge;
	}

	return framesToDealDamge;
}
