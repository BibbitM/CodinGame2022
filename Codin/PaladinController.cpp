#include "PaladinController.h"

#include "Entity.h"
#include "Game.h"
#include "Math.h"
#include "Simulate.h"
#include "Timer.h"

#include <algorithm>
#include <iostream>
#include <vector>

#define LOG_PALADIN_CONTROLLER 1

bool PaladinController::Attack(const Game& game, const Entity& danger)
{
	const int dangerFrameToAttackBase = Simulate::EnemyFramesToAttackBase(danger);
	const int heroFrameToAttackDanger = Simulate::HeroFramesToAttackEnemy(owner, danger);
	const int heroFrameToCastWind = Simulate::HeroFramesToCastSpell(owner, danger, Rules::spellWindRange);
	const int heroFrameToKill = Simulate::FramesToKill(danger.GetHealt());

	constexpr int framesCanIgnore = 5;

	// I cannot reach danger before he destroy the base.
	// TODO: add spell check.
	if (dangerFrameToAttackBase < heroFrameToAttackDanger)
		return false;

	// Check if I have time to attack other enemies.
	if (dangerFrameToAttackBase > heroFrameToAttackDanger + heroFrameToKill + framesCanIgnore)
		return false;

#if LOG_PALADIN_CONTROLLER
	std::cerr
		<< " FB:" << dangerFrameToAttackBase
		<< " FA:" << heroFrameToAttackDanger
		<< " FK:" << heroFrameToKill
		<< " FC:" << heroFrameToCastWind
		<< std::endl;
#endif

	// Check if I have to cast spell
	if (dangerFrameToAttackBase <= 2
		&& heroFrameToCastWind == 0
		&& dangerFrameToAttackBase <= heroFrameToAttackDanger + heroFrameToKill
		&& game.GetMana() > Rules::spellManaCost)
	{
		SetSpell(Spell::Wind, danger.GetId(), game.GetEnemyBasePosition(), "PC-attackWind");
		return true;
	}

	// I've to attack.
	Vector attackPosition = heroFrameToAttackDanger > 0
		? Simulate::PositionAfterFrames(danger, heroFrameToAttackDanger)
		: Simulate::GetBestAttackPosition(owner, danger, game.GetBasePosition(), game);
	SetTarget(danger.GetId(), attackPosition, "PC-attack");
	return true;
}

void PaladinController::Tick(const Game& game)
{
	FUNCTION_TIMER();

	if (wantsMoveCloserToBase && Distance2(owner.GetPosition(), game.GetBasePosition()) <= Pow2(minDistToBase))
		wantsMoveCloserToBase = false;
	else if (!wantsMoveCloserToBase && Distance2(owner.GetPosition(), game.GetBasePosition()) >= Pow2(maxDistToBase))
		wantsMoveCloserToBase = true;

	// We have already attacked dangerous enemy. No other move is needed.
	if (HasTarget())
		return;


	const Vector idlePosition = DerermineIdleMove(game);


	std::vector<const Entity*> enemies;
	enemies.reserve(game.GetAllEntities().size());

	// Get enemies in one move range that will score extra mana.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() == EntityType::Monster
			&& Distance2(enemy->GetPosition(), game.GetBasePosition()) > Pow2(Rules::baseViewRange)
			&& Simulate::HeroFramesToAttackEnemy(owner, *enemy) == 0)
		{
			enemies.push_back(enemy);
		}
	}

	if (!enemies.empty())
	{
		std::sort(enemies.begin(), enemies.end(), [this](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetPosition(), owner.GetPosition()) < Distance2(b->GetPosition(), owner.GetPosition());
		});

		const Entity* enemy = enemies.front();
		const Vector attackPosition = Simulate::GetBestAttackPosition(owner, *enemy, enemy->GetTargetPosition(), game);
		SetTarget(enemy->GetId(), attackPosition, "PC-enemy0");
		return;
	}

	/*
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
		{
			SetTarget(enemies.front()->GetId(), enemies.front()->GetPosition(), "PC-nearest");
			return;
		}
	}
	//**/

	// No enemies to attack, 
	SetTarget(-1, idlePosition, "PC-idle");
}

Vector PaladinController::DerermineIdleMove(const Game& game) const
{
	FUNCTION_TIMER();

	Vector idlePosition = owner.GetPosition();

	// Set minimal distance to the base.
	const int distToBase2 = Distance2(owner.GetPosition(), game.GetBasePosition());
	if (wantsMoveCloserToBase && distToBase2 > Pow2(minDistToBase))
		idlePosition += (game.GetBasePosition() - owner.GetPosition()).Lengthed(std::max(Sqrt(distToBase2) - minDistToBase, Rules::heroMoveRange));
	else if (!wantsMoveCloserToBase && distToBase2 < Pow2(maxDistToBase))
		idlePosition += (owner.GetPosition() - game.GetBasePosition()).Lengthed(std::max(maxDistToBase - Sqrt(distToBase2), Rules::heroMoveRange));

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
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() != EntityType::Monster)
			continue;

		const int distToEnemy2 = Distance2(enemy->GetPosition(), owner.GetPosition());
		if (distToEnemy2 < Pow2(sensDistToEnemy) && distToEnemy2 > Pow2(optDistToEnemy))
			idlePosition += (enemy->GetPosition() - owner.GetPosition()).Lengthed(Sqrt(distToEnemy2) - optDistToEnemy);
	}

	return idlePosition;
}
