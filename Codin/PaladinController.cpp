#include "PaladinController.h"

#include "Utils.h"

#include <iostream>

void PaladinController::Tick(const Game& game)
{
	UNUSED(game);
}

void PaladinController::MakeMove(std::ostream& out) const
{
	out << "WAIT";

	out << " Paladin" << std::endl;
}
