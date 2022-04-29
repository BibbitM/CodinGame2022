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
	virtual bool Attack(const Entity& danger) = 0;;
	virtual void Tick(const Game& game) = 0;
	void MakeMove(std::ostream& out) const;

protected:
	void SetTarget(int targetEntity, const Vector& targetPosition, std::string_view info);
	bool HasTarget() const { return targetEntity != -1; }
	static bool IsTargetedEntity(int entity, const Game& game);

	const Entity& owner;

private:
	std::string name{};
	Vector targetPosition{};
	int targetEntity{ -1 };
};
