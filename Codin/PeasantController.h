#pragma once

#include "Controller.h"

class PeasantController : public Controller
{
public:
	PeasantController(const Entity& owner) : Controller(owner, "Peasant") {}

protected:
	virtual void DoTick(const Game& game) override;
};
