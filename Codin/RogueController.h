#pragma once

#include "Controller.h"

#include "Rules.h"

class RogueController : public Controller
{
public:
	RogueController(const Entity& owner) : Controller(owner, "Rogue") {}

	virtual bool Attack(const Game& game, const Entity& danger, bool canCastWind) override;
	virtual bool Defend(const Game& game, const Entity& opponent, bool shouldDefend) override;
	virtual void Tick(const Game& game) override;

private:
	bool TryCastSpells(const Game& game);

	Vector GetIdleTarget(const Game& game, bool moveToOpponentsBase) const;

	Vector GetWindDirection(const Game& game) const;

	bool moveRight = false;
	int moveToOpponentsBaseForFrames = 0;

	static constexpr int optDistToBase = Rules::monsterBaseAttackRange - Rules::spellWindRange;
	static constexpr int maxDistToBase = optDistToBase + Rules::spellWindPushRange;// Rules::mapSize.y;
	static constexpr int minDistToEdge = Rules::spellWindRange;
};

