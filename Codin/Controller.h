#pragma once

#include "Vector.h"

#include <iosfwd>
#include <string>
#include <string_view>

class Entity;
class Game;

class Controller
{
public:
	Controller(const Entity& owner, std::string_view name) : owner(owner), name(name) { }
	virtual ~Controller() { }

	void Clear();
	void Tick(const Game& game);
	void MakeMove(std::ostream& out) const;

protected:
	virtual void DoTick(const Game& game) = 0;
	void SetTarget(int targetEntity, const Vector& targetPosition, std::string_view info);
	static bool IsTargetedEntity(int entity, const Game& game);

	const Entity& owner;

private:
	std::string name{};
	Vector targetPosition{};
	int targetEntity{ -1 };
};
