#include "Controller.h"

#include "Entity.h"
#include "Game.h"
#include "Utils.h"

#include <iostream>

void Controller::Clear()
{
	targetPosition = owner.GetPosition();
	targetEntity = -1;
}

void Controller::Tick(const Game& game)
{
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

void Controller::SetTarget(int entity, const Vector& pos, std::string_view info)
{
	targetEntity = entity;
	targetPosition = pos;

	TOUCH(info);
	std::cerr << "H:" << owner.GetId() << " T:" << entity << ' ' << info << std::endl;
}

bool Controller::IsTargetedEntity(int entity, const Game& game)
{
	for (auto hero : game.GetMyHeroes())
	{
		if (hero->GetController()->targetEntity == entity)
			return true;
	}
	return false;
}
