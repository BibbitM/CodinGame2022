#pragma once

#include "Vector.h"

#include <iosfwd>
#include <string>
#include <string_view>

class Entity;
class Game;

enum class Spell : int8_t
{
	None,
	Wind,
	Shield,
	Control,
};

std::ostream& operator<<(std::ostream& out, Spell spell);

class Controller
{
public:
	Controller(const Entity& owner, std::string_view name) : owner(owner), name(name) { }
	virtual ~Controller() { }

	void Clear();
	virtual bool Attack(const Game& game, const Entity& danger, bool shoulAttack) = 0;;
	virtual void Tick(const Game& game) = 0;
	void MakeMove(std::ostream& out) const;

protected:
	void SetTarget(int entity, const Vector& pos, std::string_view info);
	void SetSpell(Spell spell, int entity, const Vector& pos, std::string_view info);

	bool HasTarget() const { return targetEntity != -1; }
	static bool IsTargetedEntity(int entity, const Game& game);

	const Entity& owner;

private:
	const std::string name{};
	Vector targetPosition{};
	int targetEntity{ -1 };
	Spell targetSpell{ Spell::None };
};
