#include "PaladinController.h"

#include "Entity.h"
#include "Game.h"
#include "Math.h"
#include "Rules.h"
#include "Simulate.h"
#include "Timer.h"

#include <algorithm>
#include <iostream>
#include <vector>

#define LOG_PALADIN_CONTROLLER 1

bool PaladinController::Attack(const Entity& danger)
{
	const int dangerFrameToAttackBase = Simulate::EnemyFramesToAttackBase(danger);
	const int heroFrameToAttackDanger = Simulate::HeroFramesToAttackEnemy(owner, danger);
	const int heroFrameToKill = Simulate::FramesToKill(danger.GetHealt());

	constexpr int framesCanIgnore = 5;

	// I cannot reach danger before he destroy the base.
	// TODO: add spell check.
	if (dangerFrameToAttackBase < heroFrameToAttackDanger)
		return false;

	// Check if I have time to attack other enemies.
	if (dangerFrameToAttackBase > heroFrameToAttackDanger + heroFrameToKill + framesCanIgnore)
		return false;

	// I've to attack.
#if LOG_PALADIN_CONTROLLER
	std::cerr << "FB:" << dangerFrameToAttackBase << " FA:" << heroFrameToAttackDanger << " FK:" << heroFrameToKill << std::endl;
#endif
	SetTarget(danger.GetId(), Simulate::PositionAfterFrames(danger, heroFrameToAttackDanger), "PC-attack");
	return true;
}

void PaladinController::Tick(const Game& game)
{
	FUNCTION_TIMER();

	if (HasTarget())
		return;

	SetTarget(-1, DerermineIdleMove(game), "PC-idle");

	// TODO: Move to a function
	std::vector<Entity*> enemies;
	enemies.reserve(game.GetAllEntities().size());

	// Get only enemies.
	for (const auto& ent : game.GetAllEntities())
	{
		const auto& enemy = ent.second;
		if (enemy->GetThreatFor() == ThreatFor::MyBase)
			enemies.push_back(enemy.get());
	}

	// Find the nearest enemy and move to him.
	if (!enemies.empty())
	{
		// Sort to find nearest enemy.
		std::sort(enemies.begin(), enemies.end(), [this](Entity* a, Entity* b)
		{
			return Distance2(a->GetPosition(), owner.GetPosition()) < Distance2(b->GetPosition(), owner.GetPosition());
		});

		// Move to nearest enemy.
		if (Distance2(enemies.front()->GetPosition(), owner.GetPosition()) < Rules::heroViewRange)
			SetTarget(enemies.front()->GetId(), enemies.front()->GetPosition(), "PC-nearest");
	}
}

Vector PaladinController::DerermineIdleMove(const Game& game) const
{
	FUNCTION_TIMER();

	constexpr int minDistToBase = Rules::baseViewRange + Rules::heroViewRange * 1 / 4; // 1 / 2;
	constexpr int maxDistToBase = Rules::baseViewRange + Rules::heroViewRange * 1 / 3; // 7 / 10; // 7/10 ~= Sqrt(2) / 2 ~= 0.707107
	constexpr int optDistToBase = (minDistToBase + maxDistToBase) / 2;
	constexpr int minDistToEdge = Rules::heroViewRange * 7 / 10;
	constexpr int minDistToHero = 2 * Rules::heroViewRange * 7 / 10;
	constexpr int sensDistToEnemy = Rules::heroViewRange * 2 / 3;
	constexpr int optDistToEnemy = Rules::heroAttackRange * 1 / 2;
	constexpr int sensDistToThreat = Rules::heroViewRange;
	constexpr int optDistToThreat = Rules::heroAttackRange * 1 / 2;

	Vector idlePosition = owner.GetPosition();

	// Set minimal distance to the base.
	const int distToBase2 = Distance2(owner.GetPosition(), game.GetBasePosition());
	if (distToBase2 < Pow2(minDistToBase))
		idlePosition += (owner.GetPosition() - game.GetBasePosition()).Lengthed(optDistToBase - Sqrt(distToBase2));
	else if (distToBase2 > Pow2(maxDistToBase))
		idlePosition += (game.GetBasePosition() - owner.GetPosition()).Lengthed(Sqrt(distToBase2) - optDistToBase);

	// Set minimal distance to the map edges.
	if (owner.GetPosition().x < minDistToEdge)
		idlePosition.x += minDistToEdge - owner.GetPosition().x;
	if (owner.GetPosition().y < minDistToEdge)
		idlePosition.y += minDistToEdge - owner.GetPosition().y;
	if (Rules::mapSize.x - owner.GetPosition().x < minDistToEdge)
		idlePosition.x -= minDistToEdge - (Rules::mapSize.x - owner.GetPosition().x);
	if (Rules::mapSize.y - owner.GetPosition().y < minDistToEdge)
		idlePosition.y -= minDistToEdge - (Rules::mapSize.y - owner.GetPosition().y);

	// Set minimal distance to other players.
	for (const auto& hero : game.GetMyHeroes())
	{
		if (hero.get() == &owner)
			continue;

		const int distToHero2 = Distance2(owner.GetPosition(), hero->GetPosition());
		if (distToHero2 < Pow2(minDistToHero))
			idlePosition += owner.GetAwayDirection(*hero).Lengthed((minDistToHero - Sqrt(distToHero2)) / 2);
	}

	// Try to move to near enemies.
	for (const auto& ent : game.GetAllEntities())
	{
		const auto& enemy = ent.second;
		if (enemy.get() == &owner)
			continue;
		if (enemy->GetType() != EntityType::Monster)
			continue;

		const int distToEnemy2 = Distance2(enemy->GetPosition(), owner.GetPosition());
		if (distToEnemy2 < Pow2(sensDistToEnemy) && distToEnemy2 > Pow2(optDistToEnemy))
			idlePosition += (enemy->GetPosition() - owner.GetPosition()).Lengthed(Sqrt(distToEnemy2) - optDistToEnemy);
	}

	bool hasAnyThreat = false;

	// Try to move to near threats.
	for (const auto& ent : game.GetAllEntities())
	{
		const auto& enemy = ent.second;
		if (enemy.get() == &owner)
			continue;
		if (enemy->GetThreatFor() != ThreatFor::MyBase)
			continue;

		const int distToThreat2 = Distance2(enemy->GetPosition(), owner.GetPosition());
		if (distToThreat2 < Pow2(sensDistToThreat) && distToThreat2 > Pow2(optDistToThreat))
		{
			if (!hasAnyThreat)
			{
				idlePosition = owner.GetPosition();
				hasAnyThreat = true;
			}
			idlePosition += (enemy->GetPosition() - owner.GetPosition()).Lengthed(Sqrt(distToThreat2) - optDistToThreat);
		}
	}
	return idlePosition;
}
