#include "AssassinController.h"

#include "Entity.h"
#include "Game.h"
#include "Rules.h"
#include "Utils.h"

bool AssassinController::Attack(const Game& game, const Entity& danger, bool canCastWind)
{
	UNUSED(game);
	UNUSED(danger);
	UNUSED(canCastWind);
	return false;
}

bool AssassinController::Defend(const Game& game, const Entity& opponent, bool shouldDefend)
{
	UNUSED(game);
	UNUSED(opponent);
	UNUSED(shouldDefend);
	return false;
}

void AssassinController::Tick(const Game& game)
{
	const bool moveToOpponentsBase = moveToOpponentsBaseForFrames > 0;
	moveToOpponentsBaseForFrames = std::max(moveToOpponentsBaseForFrames - 1, 0);

	const Vector idleTarget = GetIdleTarget(game, moveToOpponentsBase);

	if (Distance2(owner.GetPosition(), game.GetOpponentsBasePosition()) < Pow2(Rules::baseViewRange + Rules::heroMoveRange))
	{
		// Try to push enemies into the base.
		if (game.GetMana() >= Rules::spellManaCost * 3)
		{
			bool castWindOnMonsters = false;
			int minDistanceToEdge = std::numeric_limits<int>::max();
			for (const auto& ent : game.GetAllEntities())
			{
				const Entity* enemy = ent.second.get();
				if (enemy->GetType() == EntityType::Monster
					&& enemy->GetShieldLife() == 0
					&& Distance2(enemy->GetPosition(), owner.GetPosition()) < Pow2(Rules::spellWindRange))
				{
					castWindOnMonsters = true;
					minDistanceToEdge = std::min(minDistanceToEdge, std::min(enemy->GetPosition().y, Rules::mapSize.y - enemy->GetPosition().y));
				}
			}

			if (castWindOnMonsters)
			{
				const Vector windTargetPoition = game.GetOpponentsBasePosition() + Vector{ 0, owner.GetPosition().y - minDistanceToEdge };
				SetSpell(Spell::Wind, -1, windTargetPoition, "AC-windMonster");
				moveToOpponentsBaseForFrames = 3;
				return;
			}
		}
	}
	
	if (Distance2(owner.GetPosition(), idleTarget) < Pow2(Rules::heroMoveRange))
	{
		// Try to protect myself with shield.
		if (game.GetMana() >= Rules::spellManaCost * 6)
		{
			if (owner.GetShieldLife() == 0)
			{
				SetSpell(Spell::Shield, owner.GetId(), {}, "AC-spellShieldHero");
				return;
			}
		}
	}

	SetTarget(-1, idleTarget, "AC-idle");
}

Vector AssassinController::GetIdleTarget(const Game& game, bool moveToOpponentsBase) const
{
	// Set minimal distance to the opponents base.
	const int distToOpponentsBase2 = Distance2(owner.GetPosition(), game.GetOpponentsBasePosition());

	if (moveToOpponentsBase
		&& distToOpponentsBase2 > Pow2(Rules::heroMoveRange + Rules::spellWindPushRange))
	{
		return game.GetOpponentsBasePosition();
	}

	const Vector idleTarget = closeToBase
		? Vector{ Rules::baseViewRange - Rules::spellWindPushRange - Rules::monsterMoveRange, Rules::spellWindRange * 7 / 10 }
		: Vector{ Rules::baseViewRange - Rules::monsterMoveRange, Rules::spellWindRange };
	if (game.GetOpponentsBasePosition() == Vector{})
		return idleTarget;
	else
		return Rules::mapSize - idleTarget;
}

