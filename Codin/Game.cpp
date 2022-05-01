#include "Game.h"

#include "PaladinController.h"
#include "PeasantController.h"
#include "Entity.h"
#include "EntityDescription.h"
#include "Rules.h"
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

	// First take care of dangerous enemies
	std::vector<const Entity*> dangerousEnemies = GetDangerousEnemies();
	std::vector<Entity*> heroes = GetHeroes();
	for (const Entity* danger : dangerousEnemies)
	{
		// Use at most two heroes to protect the base.
		if (heroes.size() <= 1)
			break;

		std::sort(heroes.begin(), heroes.end(), [danger](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetPosition(), danger->GetPosition()) < Distance2(b->GetPosition(), danger->GetPosition());
		});

		for (auto it = heroes.begin(); it != heroes.end(); /* in loop */)
		{
			if ((*it)->GetController()->Attack(*this, *danger))
			{
				const int dangerFrameToAttackBase = Simulate::EnemyFramesToAttackBase(*danger);
				const int heroFrameToAttackDanger = Simulate::HeroFramesToAttackEnemy(*(*it), *danger);
				const int heroFrameToKill = Simulate::FramesToKill(danger->GetHealt());

				it = heroes.erase(it);

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
	}

	// Tick all heroes.
	for (const auto& hero : myHeroes)
		hero->GetController()->Tick(*this);
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
	//if (myHeroes.empty())
	//	controller = std::make_unique<PeasantController>(*hero);
	//else
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

std::vector<Entity*> Game::GetHeroes()
{
	std::vector<Entity*> heroes;
	heroes.reserve(myHeroes.size());

	for (const std::shared_ptr<Entity>& hero : myHeroes)
		heroes.push_back(hero.get());

	return heroes;
}
