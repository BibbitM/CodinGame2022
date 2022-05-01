#include "Game.h"

#include "PaladinController.h"
#include "PeasantController.h"
#include "Entity.h"
#include "EntityDescription.h"
#include "Rules.h"
#include "RogueController.h"
#include "Simulate.h"
#include "StatsDescription.h"
#include "Utils.h"

#include <algorithm>
#include <cassert>
#include <iostream>

void Game::Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc)
{
	UNUSED(opponentsStats);

	++frame;

	health = myStats.health;
	mana = myStats.mana;

	// Actualize all entities.
	for (const auto& entDesc : entitiesDesc)
	{
		auto entIt = allEntities.find(entDesc.id);
		if (entIt != allEntities.end())
			entIt->second->Actualize(entDesc, frame);
		else
		{
			entIt = allEntities.insert(std::make_pair(entDesc.id, std::make_shared<Entity>(entDesc, frame))).first;
			if (entIt->second->GetType() == EntityType::MyHero)
			{
				PossesEntity(entIt->second.get());
				myHeroes.push_back(entIt->second);
			}
		}
	}

	// Remove no longer valid entities.
	for (auto it = allEntities.begin(); it != allEntities.end(); /* inside the loop */)
	{
		if (it->second->GetLastFrame() != frame)
		{
			assert(it->second->GetType() != EntityType::MyHero);
			it = allEntities.erase(it);
		}
		else
			++it;
	}

	// Prepare all heroes for new tick.
	for (const auto& hero : myHeroes)
		hero->GetController()->Clear();

	// First take care of dangerous enemies and opponents.
	TickAttackAndDefend();

	// Tick all heroes.
	for (const auto& hero : myHeroes)
		hero->GetController()->Tick(*this);
}

void Game::TickAttackAndDefend()
{
	std::vector<const Entity*> dangerousEnemies = GetDangerousEnemies();
	std::vector<const Entity*> dangerousOpponents = GetDangerousOpponents();
	std::vector<Entity*> heroes = GetHeroes();

	// Attack
	while (!dangerousEnemies.empty())
	{
		const Entity* danger = dangerousEnemies.front();
		if (heroes.empty())
			break;

		std::sort(heroes.begin(), heroes.end(), [danger](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetPosition(), danger->GetPosition()) < Distance2(b->GetPosition(), danger->GetPosition());
		});

		for (auto it = heroes.begin(); it != heroes.end(); /* in loop */)
		{
			if ((*it)->GetController()->Attack(*this, *danger, ShouldAttack(heroes)))
			{
				const int dangerFrameToAttackBase = Simulate::EnemyFramesToAttackBase(*danger);
				const int heroFrameToAttackDanger = Simulate::HeroFramesToAttackEnemy(*(*it), *danger);
				const int heroFrameToKill = Simulate::FramesToKill(danger->GetHealt());

				const bool castedWind = (*it)->GetController()->GetTargetSpell() == Spell::Wind;
				const Vector heroPosition = (*it)->GetPosition();

				it = heroes.erase(it);

				// If the wind is casted, than it affects multiple enemies.
				if (castedWind)
				{
					dangerousEnemies.erase(std::remove_if(dangerousEnemies.begin(), dangerousEnemies.end(), [heroPosition](const Entity* e)
					{
						return Distance2(e->GetPosition(), heroPosition) <= Pow2(Rules::spellWindRange);
					}), dangerousEnemies.end());
					break;
				}

				// If one hero can deal with it attack next danger.
				if (danger->GetShieldLife() == 0
					|| heroFrameToKill + heroFrameToAttackDanger <= dangerFrameToAttackBase
					|| heroes.size() <= 1)
				{
					break;
				}
			}
			else
				++it;
		}

		dangerousEnemies.erase(std::remove(dangerousEnemies.begin(), dangerousEnemies.end(), danger), dangerousEnemies.end());
	}

	// Defend
	bool anyOpponentDefneded = false;
	for (const Entity* opponent : dangerousOpponents)
	{
		if (heroes.empty())
			break;

		std::sort(heroes.begin(), heroes.end(), [opponent](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetPosition(), opponent->GetPosition()) < Distance2(b->GetPosition(), opponent->GetPosition());
		});

		for (auto it = heroes.begin(); it != heroes.end(); /* in loop */)
		{
			if ((*it)->GetController()->Defend(*this, *opponent, !anyOpponentDefneded))
			{
				anyOpponentDefneded = true;
				it = heroes.erase(it);
				break;
			}
			else
				++it;
		}
	}
}

bool Game::ShouldAttack(const std::vector<Entity*> heroes) const
{
	// For the first quarter of the game, use only one hero to protect the base.
	if (frame < Rules::gameLenght / 4 && heroes.size() <= 2)
		return false;

	// Use at most two heroes to protect the base.
	if (heroes.size() <= 1)
		return false;

	return true;
}

void Game::MakeMove(std::ostream& out) const
{
	for (const auto& hero : myHeroes)
		hero->GetController()->MakeMove(out);
}

Vector Game::GetOpponentsBasePosition() const
{
	return basePosition == Vector{} ? Rules::mapSize : Vector{};
}

void Game::PossesEntity(Entity* hero)
{
	std::unique_ptr<Controller> controller{};
	if (myHeroes.size() < 1)
		controller = std::make_unique<RogueController>(*hero);
	else
		controller = std::make_unique<PaladinController>(*hero);

	hero->SetController(std::move(controller));
}

std::vector<const Entity*> Game::GetDangerousEnemies() const
{
	std::vector<const Entity*> dangerousEnemies;
	dangerousEnemies.reserve(GetAllEntities().size());

	// Get only enemies.
	for (const auto& ent : GetAllEntities())
	{
		const auto& enemy = ent.second;
		if (enemy->GetThreatFor() == ThreatFor::MyBase)
			dangerousEnemies.push_back(enemy.get());
	}

	// Sort enemies by danger to the base.
	std::sort(dangerousEnemies.begin(), dangerousEnemies.end(), [this](const Entity* a, const Entity* b)
	{
		const int aFTDD = Simulate::EnemyFramesToAttackBase(*a);
		const int bFTDD = Simulate::EnemyFramesToAttackBase(*b);
		if (aFTDD < bFTDD)
			return true;
		if (aFTDD > bFTDD)
			return false;

		const int aDist2 = Distance2(a->GetPosition(), GetBasePosition());
		const int bDist2 = Distance2(b->GetPosition(), GetBasePosition());
		return aDist2 < bDist2;
	});

	return dangerousEnemies;
}

std::vector<const Entity*> Game::GetDangerousOpponents() const
{
	std::vector<const Entity*> dangerousOpponents;
	dangerousOpponents.reserve(3);

	// Get only enemies.
	for (const auto& opp : GetAllEntities())
	{
		const auto& opponent = opp.second;
		if (opponent->GetType() != EntityType::OpponentsHero)
			continue;
		if (Distance2(opponent->GetPosition(), GetBasePosition()) > Pow2(Rules::baseViewRange))
			continue;

		bool isAnyMonsterNearTheOpponent = false;
		for (const auto& mon : GetAllEntities())
		{
			const auto& monster = mon.second;
			if (monster->GetType() != EntityType::Monster)
				continue;
			if (Distance2(opponent->GetPosition(), monster->GetPosition()) > Pow2(Rules::spellControlRange + Rules::heroMoveRange + Rules::monsterMoveRange))
				continue;

			isAnyMonsterNearTheOpponent = true;
			break;
		}
		if (!isAnyMonsterNearTheOpponent)
			break;

		dangerousOpponents.push_back(opponent.get());
	}

	// Sort opponents by distance to the base.
	std::sort(dangerousOpponents.begin(), dangerousOpponents.end(), [this](const Entity* a, const Entity* b)
	{
		const int aDist2 = Distance2(a->GetPosition(), GetBasePosition());
		const int bDist2 = Distance2(b->GetPosition(), GetBasePosition());
		return aDist2 < bDist2;
	});

	return dangerousOpponents;
}

std::vector<Entity*> Game::GetHeroes()
{
	std::vector<Entity*> heroes;
	heroes.reserve(myHeroes.size());

	for (const std::shared_ptr<Entity>& hero : myHeroes)
		heroes.push_back(hero.get());

	return heroes;
}
