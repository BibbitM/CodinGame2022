#pragma once

#include "Controller.h"

class PaladinController : public Controller
{
public:
	PaladinController(const Entity& owner) : Controller(owner, "Paladin") {}

	virtual bool Attack(const Game& game, const Entity& danger) override;
	virtual void Tick(const Game& game) override;

private:
	Vector DerermineIdleMove(const Game& game) const;

};

