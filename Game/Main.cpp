#include "../Codin/EntityDescription.h"
#include "../Codin/Game.h"
#include "../Codin/StatsDescription.h"
#include "../Codin/Vector.h"

#include <iostream>
#include <vector>

int main()
{
	Vector base; // The corner of the map representing your base
	std::cin >> base; std::cin.ignore();
	int heroesPerPlayer; // Always 3
	std::cin >> heroesPerPlayer; std::cin.ignore();

	Game game(heroesPerPlayer);

	// game loop
	while (1)
	{
		StatsDescription myStatsDesc;
		std::cin >> myStatsDesc; std::cin.ignore();
		StatsDescription opponentStatsDesc;
		std::cin >> opponentStatsDesc; std::cin.ignore();

		int entitiesCount; // Amount of heroes and monsters you can see
		std::cin >> entitiesCount; std::cin.ignore();
		std::vector<EntityDescription> entitesDesc;
		entitesDesc.reserve(entitiesCount);
		for (int i = 0; i < entitiesCount; i++)
		{
			std::cin >> entitesDesc.emplace_back(); std::cin.ignore();
		}

		game.Tick(myStatsDesc, opponentStatsDesc, entitesDesc);
		game.MakeMove(std::cout);
	}
}
