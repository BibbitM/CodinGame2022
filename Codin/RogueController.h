#pragma once

#include "Controller.h"

#include "Rules.h"

class RogueController : public Controller
{
public:
	RogueController(const Entity& owner) : Controller(owner, "Rogue") {}

	virtual bool Attack(const Game& game, const Entity& danger, bool shouldAttack) override;
	virtual bool Defend(const Game& game, const Entity& opponent, bool shouldDefend) override;
	virtual void Tick(const Game& game) override;

private:
	bool TryCastSpells(const Game& game);
	bool TryGainMaxWildMana(const Game& game);

	Vector GetIdleTarget(const Game& game) const;

	bool moveRight = false;

	static constexpr int optDistToBase = Rules::monsterBaseAttackRange;
	static constexpr int maxDistToBase = Rules::mapSize.y;
	static constexpr int minDistToEdge = Rules::heroMoveRange * 2;
};

