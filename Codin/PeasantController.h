#pragma once

#include "Controller.h"

class PeasantController : public Controller
{
public:
	PeasantController(const Entity& owner) : Controller(owner, "Peasant") {}

	virtual bool Attack(const Game& game, const Entity& danger, bool canCastWind) override;
	virtual bool Defend(const Game& game, const Entity& opponent, bool shouldDefend) override;
	virtual void Tick(const Game& game) override;
};
