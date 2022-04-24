// ..\Codin\Vector.cpp
// #include "Vector.h" begin
// ..\Codin\Vector.h
// #pragma once
#include <iosfwd>

struct Vector
{
	int x, y;
};

std::istream& operator>>(std::istream& in, Vector& vec);
std::ostream& operator<<(std::ostream& out, const Vector& vec);
// #include "Vector.h" end

#include <iostream>

std::istream& operator>>(std::istream& in, Vector& vec)
{
	in >> vec.x >> vec.y;
	return in;
}

std::ostream& operator<<(std::ostream& out, const Vector& vec)
{
	out << vec.x << ' ' << vec.y;
	return out;
}
// Main.cpp
// #include "../Codin/Vector.h" begin
// #include "../Codin/Vector.h" end

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
		int entity_count; // Amount of heros and monsters you can see
		std::cin >> entity_count; std::cin.ignore();
		Vector sum{};
		Vector threat{};
		int threat_count = 0;
		for (int i = 0; i < entity_count; i++) {
			int id; // Unique identifier
			int type; // 0=monster, 1=your hero, 2=opponent hero
			Vector pos; // Position of this entity
			int shield_life; // Ignore for this league; Count down until shield spell fades
			int is_controlled; // Ignore for this league; Equals 1 when this entity is under a control spell
			int health; // Remaining health of this monster
			Vector vel; // Trajectory of this monster
			int near_base; // 0=monster with no target yet, 1=monster targeting a base
			int threat_for; // Given this monster's trajectory, is it a threat to 1=your base, 2=your opponent's base, 0=neither
			std::cin >> id >> type >> pos >> shield_life >> is_controlled >> health >> vel >> near_base >> threat_for; std::cin.ignore();
			sum.x += pos.x;
			sum.y += pos.y;
			if (threat_for == 1)
			{
				threat.x += pos.x;
				threat.y += pos.y;
				++threat_count;
			}
		}

		Vector center;
		center.x = sum.x / entity_count;
		center.y = sum.y / entity_count;
		threat.x = threat_count ? threat.x / threat_count : center.x;
		threat.y = threat_count ? threat.y / threat_count : center.y;

		for (int i = 0; i < heroes_per_player; i++) {

			// Write an action using cout. DON'T FORGET THE "<< endl"
			// To debug: cerr << "Debug messages..." << endl;


			// In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;
			switch (rand() % 4)
			{
			case 0:
				std::cout << "WAIT" << std::endl;
				break;
			case 1:
				std::cout << "MOVE " << center << std::endl;
				break;
			case 2:
				std::cout << "MOVE " << base << std::endl;
				break;
			case 3:
				std::cout << "MOVE " << threat << std::endl;
				break;
			}
		}
	}
}
