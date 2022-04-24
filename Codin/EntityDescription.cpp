#include "EntityDescription.h"

#include <iostream>

std::istream& operator>>(std::istream& in, EntityDescription& entDesc)
{
	in >> entDesc.id >> entDesc.type >> entDesc.pos >> entDesc.shield_life >> entDesc.is_controlled >> entDesc.health >> entDesc.vel >> entDesc.near_base >> entDesc.threat_for;
	return in;
}
