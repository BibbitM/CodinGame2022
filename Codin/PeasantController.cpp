#include "PeasantController.h"

#include "Entity.h"
#include "Game.h"
#include "Utils.h"

#include <algorithm>
#include <vector>

bool PeasantController::Attack(const Game& game, const Entity& danger, bool shoulAttack)
{
	UNUSED(game);
	if (!shoulAttack)
		return false;
	SetTarget(danger.GetId(), danger.GetTargetPosition(), "PE-attackDanger");
	return true;
}

void PeasantController::Tick(const Game& game)
{
	if (HasTarget())
		return;

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

	auto compareEnemyDist = [&game](Entity* a, Entity* b) { return Distance2(a->GetTargetPosition(), game.GetBasePosition()) < Distance2(b->GetTargetPosition(), game.GetBasePosition()); };

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
