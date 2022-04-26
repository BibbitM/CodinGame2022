#include "PaladinController.h"

#include "Entity.h"
#include "Game.h"
#include "Math.h"
#include "Rules.h"

#include <iostream>

void PaladinController::Tick(const Game& game)
{
	constexpr int minDistToBase = Rules::baseViewRange + Rules::heroViewRange * 7 / 10; // 7/10 ~= Sqrt(2) / 2 ~= 0.707107
	constexpr int minDistToEdge = Rules::heroViewRange * 7 / 10;
	constexpr int minDistToHero = 2 * Rules::heroViewRange * 7 / 10;

	targetPosition = owner.GetPosition();

	// Set minimal distance to the base.
	const int distToBaseSqr = DistanceSqr(owner.GetPosition(), game.GetBasePosition());
	if (distToBaseSqr < Sqr(minDistToBase))
		targetPosition += (owner.GetPosition() - game.GetBasePosition()).Lengthed(minDistToBase - Sqrt(distToBaseSqr));

	// Set minimal distance to the map edges.
	if (owner.GetPosition().x < minDistToEdge)
		targetPosition.x += minDistToEdge - owner.GetPosition().x;
	if (owner.GetPosition().y < minDistToEdge)
		targetPosition.y += minDistToEdge - owner.GetPosition().y;
	if (Rules::mapSize.x - owner.GetPosition().x < minDistToEdge)
		targetPosition.x -= minDistToEdge - (Rules::mapSize.x - owner.GetPosition().x);
	if (Rules::mapSize.y - owner.GetPosition().y < minDistToEdge)
		targetPosition.y -= minDistToEdge - (Rules::mapSize.y - owner.GetPosition().y);

	// Set minimal distance to other players.
	for (const auto& hero : game.GetMyHeroes())
	{
		if (hero.get() == &owner)
			continue;

		const int distaceToHeroSqr = DistanceSqr(owner.GetPosition(), hero->GetPosition());
		if (distaceToHeroSqr < Sqr(minDistToHero))
			targetPosition += (owner.GetPosition() - hero->GetPosition()).Lengthed((minDistToHero - Sqrt(distaceToHeroSqr)) / 2);
	}
}

void PaladinController::MakeMove(std::ostream& out) const
{
	if (owner.GetPosition() == targetPosition)
		out << "WAIT";
	else
		out << "MOVE " << targetPosition;

	out << " Paladin" << std::endl;
}
