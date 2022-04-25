#pragma once

#include "Controller.h"

#include "Vector.h"

#include <iosfwd>

class PeasantController : public Controller
{
public:
	PeasantController(Entity* owner) : Controller(owner) { }

	virtual void Tick(const Game& game) override;
	virtual void MakeMove(std::ostream& out) const override;

protected:
	Vector targetPosition{};
};
