#include "Controller.h"

#include "Entity.h"
#include "Game.h"

#include <algorithm>
#include <iostream>
#include <vector>

void Controller::Tick(const Game& game)
{
	std::vector<Entity*> myEnemies;
	myEnemies.reserve(game.GetAllEntities().size());
	std::vector<Entity*> otherEnemies;
	otherEnemies.reserve(game.GetAllEntities().size());

	for (const auto& ent : game.GetAllEntities())
	{
		if (ent.second->GetThreatFor() == ThreatFor::MyBase)
			myEnemies.push_back(ent.second.get());
		else if (ent.second->GetType() == EntityType::Monster)
			otherEnemies.push_back(ent.second.get());
	}

	auto compareEnemyDist = [&game](Entity* a, Entity* b) { return DistanceSqr(a->GetTargetPosition(), game.GetBasePosition()) < DistanceSqr(b->GetTargetPosition(), game.GetBasePosition()); };

	if (!myEnemies.empty())
	{
		std::sort(myEnemies.begin(), myEnemies.end(), compareEnemyDist);
		targetPosition = myEnemies.front()->GetTargetPosition();
	}
	else if (!otherEnemies.empty())
	{
		std::sort(otherEnemies.begin(), otherEnemies.end(), compareEnemyDist);
		targetPosition = otherEnemies.front()->GetTargetPosition();
	}
	else
		targetPosition = owner->GetPosition();
}

void Controller::MakeMove(std::ostream& out) const
{
	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	// In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;

	if (owner->GetPosition() == targetPosition)
		out << "WAIT" << std::endl;
	else
		out << "MOVE " << targetPosition << std::endl;
}
