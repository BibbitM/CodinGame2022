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

bool PaladinController::Attack(const Game& game, const Entity& danger, bool canCastWind)
{
	const int dangerFrameToAttackBase = Simulate::EnemyFramesToAttackBase(danger);
	const int heroFrameToAttackDanger = Simulate::HeroFramesToAttackEnemy(owner, danger);
	const int heroFrameToCastWind = Simulate::HeroFramesToCastSpell(owner, danger, Rules::spellWindRange);
	const int heroFrameToKill = Simulate::FramesToKill(danger.GetHealt());

	constexpr int framesCanIgnore = 1;

	// Set enemy move away from my base
	if (game.GetMana() >= Rules::spellManaCost * 15
		&& danger.GetShieldLife() == 0
		&& Distance2(danger.GetTargetPosition(), game.GetBasePosition()) > Pow2(Rules::monsterBaseAttackRange - Rules::monsterMoveRange / 2)
		&& Distance2(danger.GetTargetPosition(), owner.GetPosition()) < Pow2(Rules::spellControlRange))
	{
		SetSpell(Spell::Control, danger.GetId(), game.GetOpponentsBasePosition(), "PC-attackControl");
		return true;
	}

	// I cannot reach danger before he destroy the base.
	if (dangerFrameToAttackBase < heroFrameToAttackDanger)
		return false;

	const bool heroNearBase = Distance2(owner.GetPosition(), game.GetBasePosition()) <= Pow2(Rules::monsterBaseAttackRange);
	const bool dangerNearBase = Distance2(danger.GetTargetPosition(), game.GetBasePosition()) <= Pow2(Rules::monsterBaseAttackRange);

	// Check if I have time to attack other enemies.
	if (!heroNearBase && !dangerNearBase
		&& dangerFrameToAttackBase > heroFrameToAttackDanger + heroFrameToKill + framesCanIgnore)
	{
		return false;
	}

#if LOG_PALADIN_CONTROLLER
	std::cerr
		<< " FB:" << dangerFrameToAttackBase
		<< " FA:" << heroFrameToAttackDanger
		<< " FK:" << heroFrameToKill
		<< " FC:" << heroFrameToCastWind
		<< std::endl;
#endif

	// Check if I have to cast spell
	if (canCastWind
		&& danger.GetShieldLife() == 0
		&& dangerFrameToAttackBase <= 2
		&& heroFrameToCastWind == 0
		&& dangerFrameToAttackBase <= heroFrameToAttackDanger + heroFrameToKill
		&& game.GetMana() > Rules::spellManaCost)
	{
		SetSpell(Spell::Wind, danger.GetId(), game.GetOpponentsBasePosition(), "PC-attackWind");
		return true;
	}

	// Check if I have to cast spell because there is an opponent's hero in the base.
	if (canCastWind
		&& IsAnyOpponentsHeroInMyBase(game)
		&& dangerFrameToAttackBase <= 6
		&& heroFrameToCastWind == 0
		&& heroFrameToKill > 2
		&& game.GetMana() > Rules::spellManaCost * 3)
	{
		SetSpell(Spell::Wind, danger.GetId(), game.GetOpponentsBasePosition(), "PC-attackWind2");
		return true;
	}

	// Check if there is a chance to do anything with the enemy.
	if (danger.GetShieldLife() > dangerFrameToAttackBase
		&& dangerFrameToAttackBase <= 1
		&& dangerFrameToAttackBase <= heroFrameToAttackDanger + heroFrameToKill)
	{
		return false;
	}

	// I've to attack.
	Vector attackPosition = heroFrameToAttackDanger > 0
		? Simulate::PositionAfterFrames(danger, heroFrameToAttackDanger)
		: Simulate::GetBestAttackPosition(owner, danger, game.GetBasePosition(), game);
	SetTarget(danger.GetId(), attackPosition, "PC-attack");
	return true;
}

bool PaladinController::Defend(const Game& game, const Entity& opponent, bool shouldDefend)
{
	if (opponent.GetShieldLife() > 1)
		return false;
	if (!shouldDefend)
		return false;

	int minimalMana = Rules::spellManaCost * 5;
	const int opponentDistToBase2 = Distance2(opponent.GetPosition(), game.GetBasePosition());
	if (opponentDistToBase2 < Pow2(Rules::monsterBaseAttackRange))
	{
		for (const auto& ent : game.GetAllEntities())
		{
			const Entity* monster = ent.second.get();
			if (monster->GetType() != EntityType::Monster)
				continue;

			const int monsterDistToBase2 = Distance2(monster->GetPosition(), game.GetBasePosition());
			if (monsterDistToBase2 < opponentDistToBase2)
			{
				minimalMana = Rules::spellManaCost * 3;
				break;
			}
		}
	}
	if (game.GetMana() < minimalMana)
		return false;

	const int heroFrameToCastWind = Simulate::HeroFramesToCastSpell(owner, opponent, Rules::spellWindRange);

	if (heroFrameToCastWind
		|| opponent.GetShieldLife() > 0)
	{
		// I've to move closer to the opponent.
		Vector attackPosition = Simulate::GetBestAttackPosition(owner, opponent, game.GetOpponentsBasePosition(), game);
		SetTarget(opponent.GetId(), attackPosition, "PC-defend");
		return true;
	}

	SetSpell(Spell::Wind, opponent.GetId(), game.GetOpponentsBasePosition(), "PC-defendWind");
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

	if (TryCastSpellOnNearestOpponent(game))
		return;

	if (TryGainMaxWildMana(game))
		return;

	// No enemies to attack, 
	SetTarget(-1, GetIdleTarget(game), "PC-idle");
}

bool PaladinController::TryCastSpellOnNearestOpponent(const Game& game)
{
	// Check if we have enough mana.
	if (game.GetMana() < Rules::spellManaCost * 15)
		return false;

	std::vector<const Entity*> opponents;
	opponents.reserve(3);

	bool tryToCastShield = false;

	// Get opponents in spell range.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* opponent = ent.second.get();
		if (opponent->GetType() != EntityType::OpponentsHero)
			continue;

		const int opponentDist2 = Distance2(opponent->GetPosition(), owner.GetPosition());
		if (opponentDist2 <= Pow2(Rules::spellControlRange * 2))
			tryToCastShield = true;
		
		if (opponentDist2 <= Pow2(Rules::spellControlRange))
			opponents.push_back(opponent);
	}

	if (tryToCastShield && owner.GetShieldLife() == 0)
	{
		SetSpell(Spell::Shield, owner.GetId(), owner.GetPosition(), "PC-spellShield");
		return true;
	}

	if (!opponents.empty())
	{
		std::sort(opponents.begin(), opponents.end(), [this](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetPosition(), owner.GetPosition()) < Distance2(b->GetPosition(), owner.GetPosition());
		});

		for (const Entity* opponent : opponents)
		{
			if (opponent->GetShieldLife() == 0 &&
				!IsTargetedEntity(opponent->GetId(), game))
			{
				if (Distance2(opponent->GetPosition(), owner.GetPosition()) < Pow2(Rules::spellWindRange))
					SetSpell(Spell::Wind, opponent->GetId(), game.GetOpponentsBasePosition(), "PC-spellWind");
				else
					SetSpell(Spell::Control, opponent->GetId(), game.GetOpponentsBasePosition(), "PC-spellControl");
				return true;
			}
		}
	}

	return false;
}

bool PaladinController::TryGainMaxWildMana(const Game& game)
{
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
		const Vector attackPosition = Simulate::GetBestAttackPosition(owner, *enemy, GetIdleTarget(game)/*enemy->GetTargetPosition()*/, game);
		SetTarget(enemy->GetId(), attackPosition, "PC-enemy0");
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
		SetTarget(enemy->GetId(), enemy->GetTargetPosition(), "PC-enemy1");
		return true;
	}

	return false;
}

Vector PaladinController::GetIdleTarget(const Game& game) const
{
	Vector idleTarget = owner.GetPosition();

	// Set minimal distance to the base.
	const int distToBase2 = Distance2(owner.GetPosition(), game.GetBasePosition());
	if (wantsMoveCloserToBase && distToBase2 >= Pow2(minDistToBase))
		idleTarget += (game.GetBasePosition() - owner.GetPosition()).Lengthed(std::max(Sqrt(distToBase2) - minDistToBase, Rules::heroMoveRange));
	else if (!wantsMoveCloserToBase && distToBase2 <= Pow2(maxDistToBase))
		idleTarget += (owner.GetPosition() - game.GetBasePosition()).Lengthed(std::max(maxDistToBase - Sqrt(distToBase2), Rules::heroMoveRange));

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
	for (const auto& hero : game.GetMyHeroes())
	{
		if (hero.get() == &owner)
			continue;

		const int distToHero2 = Distance2(owner.GetPosition(), hero->GetPosition());
		if (distToHero2 < Pow2(minDistToHero))
			idleTarget += owner.GetAwayDirection(*hero).Lengthed((minDistToHero - Sqrt(distToHero2)) / 2);
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

bool PaladinController::IsAnyOpponentsHeroInMyBase(const Game& game) const
{
	for (const auto& opp : game.GetAllEntities())
	{
		const auto& opponent = opp.second;
		if (opponent->GetType() != EntityType::OpponentsHero)
			continue;

		if (Distance2(opponent->GetPosition(), game.GetBasePosition()) <= Pow2(Rules::monsterBaseAttackRange))
			return true;
	}

	return false;
}
