#pragma once

#include <iosfwd>
#include <vector>

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
	int numHeroes{};
};

