#pragma once

#include "PaladinController.h"

class DefenderController : public PaladinController
{
public:
	DefenderController(const Entity& owner) : PaladinController(owner, "Defender") {}

	virtual void Tick(const Game& game) override;

private:
	Vector GetIdleTarget(const Game& game) const;

	static constexpr Vector defenderPosition{ Rules::spellWindRange * 2, Rules::spellWindRange * 2 };
};

