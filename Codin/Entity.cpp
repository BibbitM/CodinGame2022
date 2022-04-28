#include "Entity.h"

#include "EntityDescription.h"

Entity::Entity(const EntityDescription& entityDesc, int frame)
{
	Actualize(entityDesc, frame);
}

Entity::Entity(const Entity& entity)
	: controllingBrain{} //< Copy resets brain.
	, position{ entity.position }
	, velocity{ entity.velocity }
	, id{ entity.id }
	, shieldLife{ entity.shieldLife }
	, health{ entity.health }
	, lastFrame{ entity.lastFrame }
	, type{ entity.type }
	, threatFor{ entity.threatFor }
	, isControlled{ entity.isControlled }
	, isNearBase{ entity.isNearBase }
{}

void Entity::Actualize(const EntityDescription& entityDesc, int frame)
{
	position = entityDesc.pos;
	velocity = entityDesc.vel;
	id = entityDesc.id;
	shieldLife = entityDesc.shieldLife;
	health = entityDesc.health;
	type = static_cast<EntityType>(entityDesc.type);
	threatFor = entityDesc.threatFor >= 0 ? static_cast<ThreatFor>(entityDesc.threatFor) : ThreatFor::None;
	isControlled = entityDesc.isControlled != 0;
	isNearBase = entityDesc.nearBase == 1;

	lastFrame = frame;
}

void Entity::SetController(std::unique_ptr<Controller> controller)
{
	controllingBrain = std::move(controller);
}
