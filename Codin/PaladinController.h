#pragma once

#include "Controller.h"
#include "Rules.h"

class PaladinController : public Controller
{
public:
	PaladinController(const Entity& owner) : Controller(owner, "Paladin") {}

	virtual bool Attack(const Game& game, const Entity& danger) override;
	virtual void Tick(const Game& game) override;

private:
	bool TryCastSpellOnNearestOpponent(const Game& game);
	bool TryGainMaxWildMana(const Game& game);

	Vector GetIdleTarget(const Game& game) const;

	bool wantsMoveCloserToBase = false;

	static constexpr int minDistToBase = Rules::baseViewRange + Rules::heroViewRange * 1 / 4; // 1 / 2;
	static constexpr int maxDistToBase = Rules::baseViewRange + Rules::heroViewRange * 2 / 2; // 7 / 10; // 7/10 ~= Sqrt(2) / 2 ~= 0.707107
	static constexpr int minDistToEdge = Rules::heroViewRange * 7 / 10;
	static constexpr int minDistToHero = 2 * Rules::heroViewRange * 7 / 10;
	static constexpr int sensDistToEnemy = Rules::heroViewRange * 3 / 2;
	static constexpr int optDistToEnemy = Rules::heroAttackRange * 1 / 2;
};

