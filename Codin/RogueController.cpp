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

	if (wantsMoveCloserToBase && Distance2(owner.GetPosition(), game.GetOpponentsBasePosition()) <= Pow2(minDistToBase))
		wantsMoveCloserToBase = false;
	else if (!wantsMoveCloserToBase && Distance2(owner.GetPosition(), game.GetOpponentsBasePosition()) >= Pow2(maxDistToBase))
		wantsMoveCloserToBase = true;

	// We have already attacked dangerous enemy. No other move is needed.
	if (HasTarget())
		return;

	if (TryCastSpells(game))
		return;

	if (TryGainMaxWildMana(game))
		return;

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

	if (closerToOpponentsBase > 0 || furtherToOpponentsBase > 1)
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
		const Vector attackPosition = Simulate::GetBestAttackPosition(owner, *enemy, GetIdleTarget(game)/*enemy->GetTargetPosition()*/, game);
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
	if (wantsMoveCloserToBase && distToOpponentsBase2 >= Pow2(minDistToBase))
		idleTarget += (game.GetOpponentsBasePosition() - owner.GetPosition()).Lengthed(std::max(Sqrt(distToOpponentsBase2) - minDistToBase, Rules::heroMoveRange));
	else if (!wantsMoveCloserToBase && distToOpponentsBase2 <= Pow2(maxDistToBase))
		idleTarget += (owner.GetPosition() - game.GetOpponentsBasePosition()).Lengthed(std::max(maxDistToBase - Sqrt(distToOpponentsBase2), Rules::heroMoveRange));

	// Set minimal distance to the map edges.
	if (owner.GetPosition().x < minDistToEdge)
		idleTarget.x += minDistToEdge - owner.GetPosition().x;
	if (owner.GetPosition().y < minDistToEdge)
		idleTarget.y += minDistToEdge - owner.GetPosition().y;
	if (Rules::mapSize.x - owner.GetPosition().x < minDistToEdge)
		idleTarget.x -= minDistToEdge - (Rules::mapSize.x - owner.GetPosition().x);
	if (Rules::mapSize.y - owner.GetPosition().y < minDistToEdge)
		idleTarget.y -= minDistToEdge - (Rules::mapSize.y - owner.GetPosition().y);

	// Set minimal distance to other players.
	for (const auto& opp : game.GetAllEntities())
	{
		const auto& opponent = opp.second;
		if (opponent->GetType() == EntityType::Monster)
			continue;

		const int distToOpponent2 = Distance2(owner.GetPosition(), opponent->GetPosition());
		if (distToOpponent2 < Pow2(minDistToHero))
			idleTarget += owner.GetAwayDirection(*opponent).Lengthed((minDistToHero - Sqrt(distToOpponent2)) / 2);
	}

	// Try to move to near enemies.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() != EntityType::Monster)
			continue;

		const int distToEnemy2 = Distance2(enemy->GetPosition(), owner.GetPosition());
		if (distToEnemy2 < Pow2(sensDistToEnemy) && distToEnemy2 > Pow2(optDistToEnemy))
			idleTarget += (enemy->GetPosition() - owner.GetPosition()).Lengthed(Sqrt(distToEnemy2) - optDistToEnemy);
	}

	return idleTarget;
}
