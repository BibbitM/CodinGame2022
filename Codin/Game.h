#pragma once

#include "Vector.h"

#include <iosfwd>
#include <memory>
#include <unordered_map>
#include <vector>

class Entity;

struct EntityDescription;
struct StatsDescription;

class Game
{
public:
	Game(const Vector& basePosition, int numHeroes)
		: basePosition(basePosition), numHeroes(numHeroes)
	{ }
	Game(const Game&) = delete;

	void Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc);

	void MakeMove(std::ostream& out) const;

	const std::unordered_map<int, std::shared_ptr<Entity>>& GetAllEntities() const { return allEntities; }
	const std::vector<std::shared_ptr<Entity>>& GetMyHeroes() const { return myHeroes; }
	const Vector& GetBasePosition() const { return basePosition; }

private:
	void PossesEntity(Entity* hero);

	std::unordered_map<int, std::shared_ptr<Entity>> allEntities;
	std::vector<std::shared_ptr<Entity>> myHeroes;
	Vector basePosition{};
	int numHeroes{};
	int frame{};
	int health{};
	int mana{};
};
