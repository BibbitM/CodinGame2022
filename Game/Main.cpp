#include "../Codin/EntityDescription.h"
#include "../Codin/Vector.h"

#include <iostream>

int main()
{
	Vector base; // The corner of the map representing your base
	std::cin >> base; std::cin.ignore();
	int heroes_per_player; // Always 3
	std::cin >> heroes_per_player; std::cin.ignore();

	// game loop
	while (1) {
		for (int i = 0; i < 2; i++) {
			int health; // Your base health
			int mana; // Ignore in the first league; Spend ten mana to cast a spell
			std::cin >> health >> mana; std::cin.ignore();
		}
		int entity_count; // Amount of heroes and monsters you can see
		std::cin >> entity_count; std::cin.ignore();
		for (int i = 0; i < entity_count; i++)
		{
			EntityDescription entDesc;
			std::cin >> entDesc; std::cin.ignore();
		}

		for (int i = 0; i < heroes_per_player; i++) {

			// Write an action using cout. DON'T FORGET THE "<< endl"
			// To debug: cerr << "Debug messages..." << endl;

			// In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;
			std::cout << "WAIT" << std::endl;
		}
	}
}
