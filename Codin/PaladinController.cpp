#include "PaladinController.h"

#include "Entity.h"
#include "Game.h"
#include "Math.h"
#include "Rules.h"

#include <algorithm>
#include <iostream>

void PaladinController::Tick(const Game& game)
{
	targetPosition = DerermineIdleMove(game);

	// TODO: Move to a function
	std::vector<Entity*> nearestEnemies;
	nearestEnemies.reserve(game.GetAllEntities().size());

	// Get only enemies.
	for (const auto& ent : game.GetAllEntities())
	{
		const auto& enemy = ent.second;
		if (enemy->GetThreatFor() == ThreatFor::MyBase)
			nearestEnemies.push_back(enemy.get());
	}

	if (!nearestEnemies.empty())
	{
		// Sort to find nearest enemy.
		std::sort(nearestEnemies.begin(), nearestEnemies.end(), [this](Entity* a, Entity* b)
		{
			return DistanceSqr(a->GetPosition(), owner.GetPosition()) < DistanceSqr(b->GetPosition(), owner.GetPosition());
		});

		// Move to nearest enemy.
		targetPosition = nearestEnemies.front()->GetPosition();
	}
}

void PaladinController::MakeMove(std::ostream& out) const
{
	if (owner.GetPosition() == targetPosition)
		out << "WAIT";
	else
		out << "MOVE " << targetPosition;

	out << " Paladin" << std::endl;
}

Vector PaladinController::DerermineIdleMove(const Game& game) const
{
	constexpr int minDistToBase = Rules::baseViewRange + Rules::heroViewRange * 1 / 2;
	constexpr int maxDistToBase = Rules::baseViewRange + Rules::heroViewRange * 7 / 10; // 7/10 ~= Sqrt(2) / 2 ~= 0.707107
	constexpr int optDistToBase = (minDistToBase + maxDistToBase) / 2;
	constexpr int minDistToEdge = Rules::heroViewRange * 7 / 10;
	constexpr int minDistToHero = 2 * Rules::heroViewRange * 7 / 10;
	constexpr int sensDistToEnemy = Rules::heroViewRange * 2 / 3;
	constexpr int optDistToEnemy = Rules::heroAttackRange * 1 / 2;
	constexpr int sensDistToThreat = Rules::heroViewRange;
	constexpr int optDistToThreat = Rules::heroAttackRange * 1 / 2;

	Vector idlePosition = owner.GetPosition();

	// Set minimal distance to the base.
	const int distToBaseSqr = DistanceSqr(owner.GetPosition(), game.GetBasePosition());
	if (distToBaseSqr < Sqr(minDistToBase))
		idlePosition += (owner.GetPosition() - game.GetBasePosition()).Lengthed(optDistToBase - Sqrt(distToBaseSqr));
	else if (distToBaseSqr > Sqr(maxDistToBase))
		idlePosition += (game.GetBasePosition() - owner.GetPosition()).Lengthed(Sqrt(distToBaseSqr) - optDistToBase);

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

		const int distToHeroSqr = DistanceSqr(owner.GetPosition(), hero->GetPosition());
		if (distToHeroSqr < Sqr(minDistToHero))
			idlePosition += (owner.GetPosition() - hero->GetPosition()).Lengthed((minDistToHero - Sqrt(distToHeroSqr)) / 2);
	}

	// Try to move to near enemies.
	for (const auto& ent : game.GetAllEntities())
	{
		const auto& enemy = ent.second;
		if (enemy.get() == &owner)
			continue;
		if (enemy->GetType() != EntityType::Monster)
			continue;

		const int distToEnemySqr = DistanceSqr(enemy->GetPosition(), owner.GetPosition());
		if (distToEnemySqr < Sqr(sensDistToEnemy) && distToEnemySqr > Sqr(optDistToEnemy))
			idlePosition += (enemy->GetPosition() - owner.GetPosition()).Lengthed(Sqrt(distToEnemySqr) - optDistToEnemy);
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

		const int distToThreatSqr = DistanceSqr(enemy->GetPosition(), owner.GetPosition());
		if (distToThreatSqr < Sqr(sensDistToThreat) && distToThreatSqr > Sqr(optDistToThreat))
		{
			if (!hasAnyThreat)
			{
				idlePosition = owner.GetPosition();
				hasAnyThreat = true;
			}
			idlePosition += (enemy->GetPosition() - owner.GetPosition()).Lengthed(Sqrt(distToThreatSqr) - optDistToThreat);
		}
	}
	return idlePosition;
}
