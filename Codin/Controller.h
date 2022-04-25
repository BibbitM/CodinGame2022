#pragma once

#include <iosfwd>

class Entity;
class Game;

class Controller
{
public:
	Controller(Entity* owner) : owner(owner) { }
	virtual ~Controller() { }

	virtual void Tick(const Game& game) = 0;
	virtual void MakeMove(std::ostream& out) const = 0;

protected:
	Entity* owner{};
};
