#pragma once

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
	Game(int numHeroes)
		: numHeroes(numHeroes)
	{ }

	void Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc);

	void MakeMove(std::ostream& out) const;

private:
	std::unordered_map<int, std::shared_ptr<Entity>> allEntities;
	std::vector<std::shared_ptr<Entity>> myHeroes;
	int numHeroes{};
	int frame{};
};
