#pragma once

#include "Vector.h"

#include <iosfwd>
#include <memory>
#include <unordered_map>

class Entity;
class Game;

class Controller
{
public:
	Controller(Entity* owner) : owner(owner) { }

	void Tick(const Game& game);
	void MakeMove(std::ostream& out) const;

private:
	Entity* owner{};
	Vector targetPosition{};
};
