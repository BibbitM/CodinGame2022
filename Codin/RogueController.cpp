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

bool RogueController::Attack(const Game& game, const Entity& danger, bool canCastWind)
{
	UNUSED(game);
	UNUSED(danger);
	UNUSED(canCastWind);
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

	const bool moveToOpponentsBase = moveToOpponentsBaseForFrames > 0;
	moveToOpponentsBaseForFrames = std::max(moveToOpponentsBaseForFrames - 1, 0);

	// We have already attacked dangerous enemy. No other move is needed.
	if (HasTarget())
		return;

	if (TryCastSpells(game))
		return;

	// No enemies to attack, 
	SetTarget(-1, GetIdleTarget(game, moveToOpponentsBase), "RC-idle");
}

bool RogueController::TryCastSpells(const Game& game)
{
	// Check if we have enough mana.
	if (game.GetMana() >= Rules::spellManaCost * 5)
	{
		// Check if there are enemies in wind range.
		int closerToOpponentsBase = 0;
		int furtherToOpponentsBase = 0;

		const int ownerDistToOpponentsBase2 = Distance2(owner.GetPosition(), game.GetOpponentsBasePosition());
		if (ownerDistToOpponentsBase2 >= Pow2(maxDistToBase))
			return false;

		// Get enemies in one move range that will score extra mana.
		for (const auto& ent : game.GetAllEntities())
		{
			const Entity* enemy = ent.second.get();
			const int enemyDistToOpponentsBase2 = Distance2(enemy->GetPosition(), game.GetOpponentsBasePosition());
			if (enemy->GetType() == EntityType::Monster
				&& Distance2(enemy->GetPosition(), owner.GetPosition()) <= Pow2(Rules::spellWindRange)
				&& (enemy->GetThreatFor() == ThreatFor::OpponentsBase
					|| enemyDistToOpponentsBase2 < Pow2(Rules::monsterBaseAttackRange + Rules::spellWindPushRange)
					)
				)
			{
				if (enemyDistToOpponentsBase2 < ownerDistToOpponentsBase2)
					++closerToOpponentsBase;
				else
					++furtherToOpponentsBase;
			}
		}

		if (ownerDistToOpponentsBase2 < Pow2(optDistToBase + Rules::spellWindRange)
			&& (closerToOpponentsBase > 0 || furtherToOpponentsBase > 0))
		{
			SetSpell(Spell::Wind, -1, GetWindDirection(game), "RC-spellWind");
			moveToOpponentsBaseForFrames = 3;
			return true;
		}
	}

	// Check if we have enough mana.a
	if (game.GetMana() >= Rules::spellManaCost * 10)
	{
		// Cast Shield on an opponent in the base to prevent pushing them inside the base.
		for (const auto& opp : game.GetAllEntities())
		{
			const Entity* opponent = opp.second.get();
			if (opponent->GetType() != EntityType::OpponentsHero)
				continue;
			if (opponent->GetShieldLife() > 0)
				continue;
			if (Distance2(opponent->GetPosition(), owner.GetPosition()) >= Pow2(Rules::spellShieldRange))
				continue;
			if (Distance2(opponent->GetPosition(), game.GetOpponentsBasePosition()) >= Pow2(Rules::monsterBaseAttackRange))
				continue;

			SetSpell(Spell::Shield, opponent->GetId(), opponent->GetPosition(), "RC-spellShieldOpponent");
			return true;
		}
	}

	return false;
}

Vector RogueController::GetIdleTarget(const Game& game, bool moveToOpponentsBase) const
{
	Vector idleTarget = owner.GetPosition();

	// Set minimal distance to the opponents base.
	const int distToOpponentsBase2 = Distance2(owner.GetPosition(), game.GetOpponentsBasePosition());

	if (moveToOpponentsBase
		&& distToOpponentsBase2 > Pow2(Rules::heroMoveRange + Rules::spellWindPushRange))
	{
		return game.GetOpponentsBasePosition();
	}

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

Vector RogueController::GetWindDirection(const Game& game) const
{
	return game.GetOpponentsBasePosition();
	//Vector basePosition = game.GetOpponentsBasePosition();
	//if (Distance2(owner.GetPosition(), basePosition) <= Pow2(Rules::spellWindRange + 1))
	//	return basePosition;

	//if (basePosition == Vector{})
	//	basePosition = Vector{ Rules::spellWindRange, Rules::spellWindRange };
	//else
	//	basePosition -= Vector{ Rules::spellWindRange, Rules::spellWindRange };

	//return basePosition;
}
