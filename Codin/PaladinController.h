#pragma once

#include "Controller.h"

class PaladinController : public Controller
{
public:
	PaladinController(const Entity& owner) : Controller(owner, "Paladin") {}

protected:
	virtual void DoTick(const Game& game) override;

private:
	Vector DerermineIdleMove(const Game& game) const;

};

