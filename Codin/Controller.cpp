#include "Controller.h"

#include "Utils.h"

#include <iostream>

void Controller::Tick(const std::unordered_map<int, std::shared_ptr<Entity>>& allEntities)
{
	UNUSED(allEntities);
}

void Controller::MakeMove(std::ostream& out) const
{
	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	// In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;
	out << "WAIT" << std::endl;
}
