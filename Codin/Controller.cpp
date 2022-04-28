#include "Controller.h"

#include "Entity.h"

#include <iostream>

void Controller::Tick(const Game& game)
{
	// Clear state.
	targetPosition = owner.GetPosition();
	targetEntity = -1;

	DoTick(game);
}

void Controller::MakeMove(std::ostream& out) const
{
	if (owner.GetPosition() == targetPosition)
		out << "WAIT";
	else
		out << "MOVE " << targetPosition;

	out << ' ' << name << std::endl;
}

void Controller::SetTarget(int entity, const Vector& pos)
{
	targetEntity = entity;
	targetPosition = pos;
}
