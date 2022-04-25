#pragma once

#include "Controller.h"

class PaladinController : public Controller
{
public:
	PaladinController(Entity* owner) : Controller(owner) { }

	virtual void Tick(const Game& game) override;
	virtual void MakeMove(std::ostream& out) const override;

};

