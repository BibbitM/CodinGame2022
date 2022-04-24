#pragma once

#include "Vector.h"

#include <cstdint>

struct EntityDescription;

enum class EntityType : uint8_t
{
	Monster,
	MyHero,
	OpponentsHero,
};

enum class ThreatFor : uint8_t
{
	None,
	MyBase,
	OpponentsBase,
};

class Entity
{
public:
	Entity(const EntityDescription& entityDesc, int frame);
	void Actualize(const EntityDescription& entityDesc, int frame);

	EntityType GetType() const { return type; }
	int GetLastFrame() const { return lastFrame; }

private:
	Vector position{};
	Vector velocity{};
	int id{};
	int shieldLife{};
	int health{};
	int lastFrame{};
	EntityType type{};
	ThreatFor threatFor{};
	bool isControlled{};
	bool isNearBase{};
};
