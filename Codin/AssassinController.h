#pragma once

#include "Controller.h"

class AssassinController : public Controller
{
public:
	AssassinController(const Entity& owner, bool closeToBase) : Controller(owner, "Assassin"), closeToBase(closeToBase) {}

	virtual bool Attack(const Game& game, const Entity& danger, bool canCastWind) override;
	virtual bool Defend(const Game& game, const Entity& opponent, bool shouldDefend) override;
	virtual void Tick(const Game& game) override;

private:
	Vector GetIdleTarget(const Game& game, bool moveToOpponentsBase) const;

	int moveToOpponentsBaseForFrames = 0;
	bool closeToBase = false;
};
