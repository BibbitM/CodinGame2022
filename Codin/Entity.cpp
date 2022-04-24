#include "Entity.h"

#include "EntityDescription.h"

Entity::Entity(const EntityDescription& entityDesc, int frame)
{
	Actualize(entityDesc, frame);
}

void Entity::Actualize(const EntityDescription& entityDesc, int frame)
{
	position = entityDesc.pos;
	velocity = entityDesc.vel;
	id = entityDesc.id;
	shieldLife = entityDesc.shieldLife;
	health = entityDesc.health;
	type = static_cast<EntityType>(entityDesc.type);
	threatFor = static_cast<ThreatFor>(entityDesc.threatFor);
	isControlled = entityDesc.isControlled != 0;
	isNearBase = entityDesc.nearBase != 0;

	lastFrame = frame;
}

void Entity::SetController(std::unique_ptr<Controller> controller)
{
	controllingBrain = std::move(controller);
}
