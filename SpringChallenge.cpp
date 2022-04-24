// ..\Codin\EntityDescription.cpp
// #include "EntityDescription.h" begin
// ..\Codin\EntityDescription.h
// #pragma once
// #include "Vector.h" begin
// ..\Codin\Vector.h
// #pragma once
#include <iosfwd>

struct Vector
{
	int x, y;

	bool operator==(const Vector& b) const { return x == b.x && y == b.y; }

	Vector operator+(const Vector& b) const { return Vector{ x + b.x, y + b.y }; }
	Vector& operator+=(const Vector& b) { x += b.x; y += b.y; return *this; }

	Vector operator-(const Vector& b) const { return Vector{ x - b.x, y - b.y }; }
	Vector& operator-=(const Vector& b) { x -= b.x; y -= b.y; return *this; }

	Vector operator*(int b) const { return Vector{ x * b, y * b }; }
	Vector& operator*=(int b) { x *= b; y *= b; return *this; }

	Vector operator/(int b) const { return Vector{ x / b, y / b }; }
	Vector& operator/=(int b) { x /= b; y /= b; return *this; }
};

std::istream& operator>>(std::istream& in, Vector& vec);
std::ostream& operator<<(std::ostream& out, const Vector& vec);
// #include "Vector.h" end

#include <iosfwd>

struct EntityDescription
{
	int id; // Unique identifier
	int type; // 0=monster, 1=your hero, 2=opponent hero
	Vector pos; // Position of this entity
	int shield_life; // Ignore for this league; Count down until shield spell fades
	int is_controlled; // Ignore for this league; Equals 1 when this entity is under a control spell
	int health; // Remaining health of this monster
	Vector vel; // Trajectory of this monster
	int near_base; // 0=monster with no target yet, 1=monster targeting a base
	int threat_for; // Given this monster's trajectory, is it a threat to 1=your base, 2=your opponent's base, 0=neither
};

std::istream& operator>>(std::istream& in, EntityDescription& entDesc);
// #include "EntityDescription.h" end

#include <iostream>

std::istream& operator>>(std::istream& in, EntityDescription& entDesc)
{
	in >> entDesc.id >> entDesc.type >> entDesc.pos >> entDesc.shield_life >> entDesc.is_controlled >> entDesc.health >> entDesc.vel >> entDesc.near_base >> entDesc.threat_for;
	return in;
}
// ..\Codin\Vector.cpp
// #include "Vector.h" begin
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
// #include "../Codin/EntityDescription.h" begin
// #include "../Codin/EntityDescription.h" end
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
