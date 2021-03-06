#include "EntityDescription.h"

#include <iostream>

std::istream& operator>>(std::istream& in, EntityDescription& entDesc)
{
	in >> entDesc.id >> entDesc.type >> entDesc.pos >> entDesc.shieldLife >> entDesc.isControlled >> entDesc.health >> entDesc.vel >> entDesc.nearBase >> entDesc.threatFor;
	return in;
}

std::ostream& operator<<(std::ostream& out, const EntityDescription& entDesc)
{
	out << entDesc.id << ' ' << entDesc.type << ' ' << entDesc.pos << ' ' << entDesc.shieldLife << ' ' << entDesc.isControlled << ' ' << entDesc.health << ' ' << entDesc.vel << ' ' << entDesc.nearBase << ' ' << entDesc.threatFor;
	return out;
}
