#pragma once

#include "Controller.h"

class PeasantController : public Controller
{
public:
	PeasantController(const Entity& owner) : Controller(owner, "Peasant") {}

	virtual bool Attack(const Entity& danger) override;
	virtual void Tick(const Game& game) override;
};
