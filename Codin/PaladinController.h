#pragma once

#include "Controller.h"

#include "Vector.h"

class PaladinController : public Controller
{
public:
	PaladinController(const Entity& owner) : Controller(owner) { }

	virtual void Tick(const Game& game) override;
	virtual void MakeMove(std::ostream& out) const override;

private:
	Vector targetPosition{};
};

