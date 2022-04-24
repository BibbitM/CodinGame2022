#include "Game.h"

#include "Entity.h"
#include "EntityDescription.h"
#include "Utils.h"

#include <algorithm>
#include <cassert>
#include <iostream>

void Game::Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc)
{
	UNUSED(myStats);
	UNUSED(opponentsStats);

	++frame;

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
				myHeroes.push_back(entIt->second);
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
}

void Game::MakeMove(std::ostream& out) const
{
	for (int i = 0; i < numHeroes; i++) {

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;

		// In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;
		out << "WAIT" << std::endl;
	}
}
