#include "Controller.h"

#include "Entity.h"
#include "Game.h"
#include "Utils.h"

#include <iostream>

#define LOG_TARGET 1

void Controller::Clear()
{
	targetPosition = owner.GetPosition();
	targetEntity = -1;
	targetSpell = Spell::None;
}

void Controller::MakeMove(std::ostream& out) const
{
	if (targetSpell != Spell::None)
	{
		out << "SPELL " << targetSpell;
		if (targetSpell == Spell::Shield || targetSpell == Spell::Control)
			out << ' ' << targetEntity;
		if (targetSpell == Spell::Wind || targetSpell == Spell::Control)
			out << ' ' << targetPosition;
	}
	else if (owner.GetPosition() != targetPosition)
		out << "MOVE " << targetPosition;
	else
		out << "WAIT";

	out << ' ' << name << owner.GetId() << std::endl;
}

void Controller::SetTarget(int entity, const Vector& pos, std::string_view info)
{
	targetEntity = entity;
	targetPosition = pos;
	targetSpell = Spell::None;

#if LOG_TARGET
	std::cerr << "MH:" << owner.GetId() << " T:" << entity << ' ' << info << std::endl;
#else
	TOUCH(info);
#endif
}

void Controller::SetSpell(Spell spell, int entity, const Vector& pos, std::string_view info)
{
	targetEntity = entity;
	targetPosition = pos;
	targetSpell = spell;

#if LOG_TARGET
	std::cerr << "SH(" << spell << "):" << owner.GetId() << " T:" << entity << " P:" << pos << ' ' << info << std::endl;
#else
	TOUCH(info);
#endif

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

std::ostream& operator<<(std::ostream& out, Spell spell)
{
	std::string_view spellName{};
	switch (spell)
	{
	case Spell::Wind: spellName = "WIND"; break;
	case Spell::Shield: spellName = "SHIELD"; break;
	case Spell::Control: spellName = "CONTROL"; break;
	}
	return out << spellName;
}
