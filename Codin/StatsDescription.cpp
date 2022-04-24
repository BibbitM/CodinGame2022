#include "StatsDescription.h"

#include <iostream>

std::istream& operator>>(std::istream& in, StatsDescription& statsDesc)
{
	in >> statsDesc.health >> statsDesc.mana;
	return in;
}
