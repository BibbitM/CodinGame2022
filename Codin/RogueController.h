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

	bool wantsMoveCloserToBase = false;

	static constexpr int minDistToBase = Rules::baseViewRange - Rules::spellWindRange;
	static constexpr int maxDistToBase = Rules::baseViewRange + Rules::spellWindRange;
	static constexpr int minDistToEdge = Rules::heroViewRange * 7 / 10;
	static constexpr int minDistToHero = 2 * Rules::heroViewRange * 7 / 10;
	static constexpr int sensDistToEnemy = Rules::heroViewRange * 3 / 2;
	static constexpr int optDistToEnemy = Rules::heroAttackRange * 1 / 2;
};

