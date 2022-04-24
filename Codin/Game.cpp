#include "Game.h"

#include "Utils.h"

#include <iostream>

void Game::Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc)
{
	UNUSED(myStats);
	UNUSED(opponentsStats);

	UNUSED(entitiesDesc);
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
