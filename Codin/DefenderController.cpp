#include "DefenderController.h"

#include "Game.h"

void DefenderController::Tick(const Game& game)
{
	// We have already attacked dangerous enemy. No other move is needed.
	if (HasTarget())
		return;

	// No enemies to attack, 
	SetTarget(-1, GetIdleTarget(game), "DC-idle");
}

Vector DefenderController::GetIdleTarget(const Game& game) const
{
	Vector idleTarget = game.GetBasePosition();

	if (idleTarget == Vector{})
		idleTarget = defenderPosition;
	else
		idleTarget -= defenderPosition;

	return idleTarget;
}
