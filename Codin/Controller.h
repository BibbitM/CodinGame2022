#pragma once

#include "Vector.h"

#include <iosfwd>
#include <string>

class Entity;
class Game;

class Controller
{
public:
	Controller(const Entity& owner, std::string_view name) : owner(owner), name(name) { }
	virtual ~Controller() { }

	void Tick(const Game& game);
	void MakeMove(std::ostream& out) const;

protected:
	virtual void DoTick(const Game& game) = 0;

	const Entity& owner;

	std::string name{};
	Vector targetPosition{};
};
