#include "PeasantController.h"

#include "Entity.h"
#include "Game.h"

#include <algorithm>
#include <vector>

void PeasantController::DoTick(const Game& game)
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
		SetTarget(myEnemies.front()->GetId(), myEnemies.front()->GetTargetPosition(), "PE-myEnemy");
	}
	else if (!otherEnemies.empty())
	{
		std::sort(otherEnemies.begin(), otherEnemies.end(), compareEnemyDist);
		SetTarget(otherEnemies.front()->GetId(), otherEnemies.front()->GetTargetPosition(), "PE-otherEnemy");
	}
}
