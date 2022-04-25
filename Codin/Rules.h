#pragma once

#include "Vector.h"

struct Rules
{
	static constexpr Vector mapSize{ 17630, 9000 };

	static constexpr int baseViewRange = 6000;

	static constexpr int heroViewRange = 2200;
	static constexpr int heroMoveRange = 800;
	static constexpr int heroAttackRange = 800;
	static constexpr int heroDamage = 2;

	static constexpr int monsterMoveRange = 400;
	static constexpr int monsterBaseAttackRange = 5000;
	static constexpr int monsterBaseDestroyRange = 300;

	static constexpr int spellManaCost = 10;
	static constexpr int spellWindPushRange = 2200;
	static constexpr int spellWindRange = 1280;
	static constexpr int spellShieldTime = 12;
	static constexpr int spellShieldRange = 2200;
	static constexpr int spellControlRange = 2200;
};
