#include "RogueController.h"

#include "Entity.h"
#include "Game.h"
#include "Math.h"
#include "Simulate.h"
#include "Timer.h"

#include <algorithm>
#include <iostream>
#include <vector>

#define LOG_ROGUE_CONTROLLER 1

bool RogueController::Attack(const Game& game, const Entity& danger, bool shouldAttack)
{
	UNUSED(game);
	UNUSED(danger);
	UNUSED(shouldAttack);
	return false;
}

bool RogueController::Defend(const Game& game, const Entity& opponent, bool shouldDefend)
{
	UNUSED(game);
	UNUSED(opponent);
	UNUSED(shouldDefend);
	return false;
}

void RogueController::Tick(const Game& game)
{
	FUNCTION_TIMER();

	if (Distance2(owner.GetPosition(), game.GetOpponentsBasePosition()) <= Pow2(maxDistToBase))
	{
		if (moveRight
			&& (owner.GetPosition().x < minDistToEdge || Rules::mapSize.x - owner.GetPosition().x < minDistToEdge))
		{
			moveRight = false;
		}
		else if (!moveRight
			&& (owner.GetPosition().y < minDistToEdge || Rules::mapSize.y - owner.GetPosition().y < minDistToEdge))
		{
			moveRight = true;
		}
	}

	// We have already attacked dangerous enemy. No other move is needed.
	if (HasTarget())
		return;

	if (TryCastSpells(game))
		return;

	//if (TryGainMaxWildMana(game))
	//	return;

	// No enemies to attack, 
	SetTarget(-1, GetIdleTarget(game), "RC-idle");
}

bool RogueController::TryCastSpells(const Game& game)
{
	// Check if we have enough mana.
	if (game.GetMana() < Rules::spellManaCost * 3)
		return false;

	// Check if there are enemies in wind range.
	int closerToOpponentsBase = 0;
	int furtherToOpponentsBase = 0;

	const int ownerDistToOpponentsBase2 = Distance2(owner.GetPosition(), game.GetOpponentsBasePosition());

	// Get enemies in one move range that will score extra mana.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() == EntityType::Monster
			&& Distance2(enemy->GetPosition(), owner.GetPosition()) <= Pow2(Rules::spellWindRange))
		{
			if (Distance2(enemy->GetPosition(), game.GetOpponentsBasePosition()) < ownerDistToOpponentsBase2)
				++closerToOpponentsBase;
			else
				++furtherToOpponentsBase;
		}
	}

	if (ownerDistToOpponentsBase2 < Pow2(optDistToBase + Rules::spellWindRange)
		&& (closerToOpponentsBase > 0 || furtherToOpponentsBase > 0))
	{
		SetSpell(Spell::Wind, -1, game.GetOpponentsBasePosition(), "RC-spellWindM");
		return true;
	}

	return false;
}

bool RogueController::TryGainMaxWildMana(const Game& game)
{
	std::vector<const Entity*> enemies;
	enemies.reserve(game.GetAllEntities().size());

	// Get enemies in one move range that will score extra mana.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() == EntityType::Monster
			&& Distance2(enemy->GetPosition(), game.GetBasePosition()) > Pow2(Rules::baseViewRange)
			&& Distance2(enemy->GetPosition(), game.GetOpponentsBasePosition()) > Pow2(Rules::monsterBaseAttackRange)
			&& Distance2(enemy->GetTargetPosition(), game.GetOpponentsBasePosition()) > Pow2(Rules::monsterBaseAttackRange)
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
		const Vector attackPosition = Simulate::GetBestAttackPosition(owner, *enemy, GetIdleTarget(game), game);
		SetTarget(enemy->GetId(), attackPosition, "RC-enemy0");
		return true;
	}

	enemies.clear();

	// Get enemies in two move range that will score extra mana.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() == EntityType::Monster
			&& Distance2(enemy->GetPosition(), game.GetBasePosition()) > Pow2(Rules::baseViewRange)
			&& Distance2(enemy->GetTargetPosition(), game.GetBasePosition()) > Pow2(Rules::baseViewRange)
			&& Distance2(enemy->GetPosition(), game.GetOpponentsBasePosition()) > Pow2(Rules::monsterBaseAttackRange)
			&& Distance2(enemy->GetTargetPosition(), game.GetOpponentsBasePosition()) > Pow2(Rules::monsterBaseAttackRange)
			&& Simulate::HeroFramesToAttackEnemy(owner, *enemy) == 1)
		{
			enemies.push_back(enemy);
		}
	}

	if (!enemies.empty())
	{
		std::sort(enemies.begin(), enemies.end(), [this](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetTargetPosition(), owner.GetPosition()) < Distance2(b->GetTargetPosition(), owner.GetPosition());
		});

		const Entity* enemy = enemies.front();
		SetTarget(enemy->GetId(), enemy->GetTargetPosition(), "RC-enemy1");
		return true;
	}

	return false;
}

Vector RogueController::GetIdleTarget(const Game& game) const
{
	Vector idleTarget = owner.GetPosition();

	// Set minimal distance to the opponents base.
	const int distToOpponentsBase2 = Distance2(owner.GetPosition(), game.GetOpponentsBasePosition());

	const Vector fromOpponentBase = owner.GetPosition() != game.GetOpponentsBasePosition()
		? (owner.GetPosition() - game.GetOpponentsBasePosition()).Lengthed(Rules::heroMoveRange)
		: (game.GetBasePosition() - game.GetOpponentsBasePosition()).Lengthed(Rules::heroMoveRange);
	const Vector rightOpponentBase = fromOpponentBase.Perpendicular() * 4;

	if (distToOpponentsBase2 > Pow2(maxDistToBase))
	{
		idleTarget -= fromOpponentBase;
	}
	else
	{
		if (distToOpponentsBase2 > Pow2(optDistToBase))
			idleTarget -= fromOpponentBase;
		else if (distToOpponentsBase2 < Pow2(optDistToBase))
			idleTarget += fromOpponentBase;

		if (moveRight)
			idleTarget += rightOpponentBase;
		else
			idleTarget -= rightOpponentBase;
	}

	return idleTarget;
}
