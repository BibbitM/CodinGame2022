#include "Simulate.h"

#include "Entity.h"
#include "Game.h"
#include "Rules.h"

#include <iostream>

#define LOG_SIMULATE_BEST_ATTACK 1

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

Vector Simulate::PositionAfterFrames(const Entity& entity, int frames)
{
	if (frames == 0)
		return entity.GetPosition();

	Entity simulatedEnemy{ entity };
	for (int i = 0; i < frames; ++i)
		Update(simulatedEnemy);

	return simulatedEnemy.GetPosition();
}

int Simulate::EnemyFramesToAttackBase(const Entity& enemy)
{
	if (enemy.GetThreatFor() == ThreatFor::None)
		return std::numeric_limits<int>::max();

	const Vector basePosition = GetNearestBasePosition(enemy);

	constexpr int maxFrames2 = static_cast<int>(static_cast<float>(Rules::mapSize.Length2()) / static_cast<float>(Pow2(Rules::monsterMoveRange)));
	int framesToDealDamge = 0;
	Entity simulatedEnemy{ enemy };
	while (Distance2(simulatedEnemy.GetTargetPosition(), basePosition) > Pow2(Rules::monsterBaseDestroyRange)
		&& Pow2(framesToDealDamge) < maxFrames2)
	{
		Update(simulatedEnemy);
		++framesToDealDamge;
	}

	return framesToDealDamge;
}

int Simulate::HeroFramesToAttackEnemy(const Entity& hero, const Entity& enemy)
{
	constexpr int maxFrames2 = static_cast<int>(static_cast<float>(Rules::mapSize.Length2()) / static_cast<float>(Pow2(Rules::monsterMoveRange)));
	int framesToAttack = 0;
	Entity simulatedEnemy{ enemy };
	while (Distance2(hero.GetPosition(), enemy.GetPosition()) >= Pow2(Rules::heroMoveRange * (framesToAttack + 1) + Rules::heroAttackRange)
		&& Pow2(framesToAttack) < maxFrames2)
	{
		Update(simulatedEnemy);
		++framesToAttack;
	}

	return framesToAttack;
}

int Simulate::HeroFramesToCastSpell(const Entity& hero, const Entity& enemy, int spellRange)
{
	constexpr int maxFrames2 = static_cast<int>(static_cast<float>(Rules::mapSize.Length2()) / static_cast<float>(Pow2(Rules::monsterMoveRange)));
	int framesToSpell = 0;
	Entity simulatedEnemy{ enemy };
	while (Distance2(hero.GetPosition(), enemy.GetPosition()) >= Pow2(Rules::heroMoveRange * framesToSpell + spellRange)
		&& Pow2(framesToSpell) < maxFrames2)
	{
		Update(simulatedEnemy);
		++framesToSpell;
	}

	return framesToSpell;
}

int Simulate::FramesToKill(int healt)
{
	return healt / Rules::heroDamage;
}

Vector Simulate::GetBestAttackPosition(const Entity& hero, const Entity& danger, const Vector& preferedPosition, const Game& game)
{
	std::vector<const Entity*> nearestEnemies;
	nearestEnemies.reserve(16);

	// Find the nearest enemies that hero can attack together with the enemy.
	for (auto it : game.GetAllEntities())
	{
		const Entity* enemy = it.second.get();
		if (enemy->GetType() != EntityType::Monster)
			continue;
		if (Distance2(danger.GetPosition(), enemy->GetPosition()) > Pow2(Rules::heroAttackRange * 2))
			continue;

		nearestEnemies.push_back(enemy);
	}

	while (nearestEnemies.size() > 1)
	{
		Vector nearestPosition = GetPreferedAttackPosition(hero, danger, GetEnemiesCenter(nearestEnemies));

		bool canAttackAllNearest = true;
		for (auto it = nearestEnemies.begin(); it != nearestEnemies.end(); /* in loop */)
		{
			if (Distance2(nearestPosition, (*it)->GetPosition()) <= Pow2(Rules::heroAttackRange))
				++it;
			else
			{
				it = nearestEnemies.erase(it);
				canAttackAllNearest = false;
			}
		}

		if (canAttackAllNearest)
		{
#if LOG_SIMULATE_BEST_ATTACK
			std::cerr << " SBA:" << nearestEnemies.size() << std::endl;
#endif
			return nearestPosition;
		}
	}

	// There is no possibility of attacking more enemies than one.
	return GetPreferedAttackPosition(hero, danger, preferedPosition);
}

Vector Simulate::GetPreferedAttackPosition(const Entity& hero, const Entity& danger, const Vector& preferedPosition)
{
	Vector position = preferedPosition;

	constexpr int maxSteps = 10;
	int step = 0;
	while (step < maxSteps
		&& (Distance2(position, hero.GetPosition()) > Pow2(Rules::heroMoveRange)
			|| Distance2(position, danger.GetPosition()) > Pow2(Rules::heroAttackRange)))
	{
		position = hero.GetPosition() + (position - hero.GetPosition()).Limited(Rules::heroMoveRange);
		position = danger.GetPosition() + (position - danger.GetPosition()).Limited(Rules::heroAttackRange);
		++step;
	}

	return position;
}

Vector Simulate::GetEnemiesCenter(const std::vector<const Entity*>& enemies)
{
	Vector center{};
	for (const Entity* e : enemies)
		center += e->GetPosition();
	return center / std::max(static_cast<int>(enemies.size()), 1);
}
