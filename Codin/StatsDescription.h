#pragma once

#include <iosfwd>

struct StatsDescription
{
	int health; // Your base health
	int mana; // Ignore in the first league; Spend ten mana to cast a spell
};

std::istream& operator>>(std::istream& in, StatsDescription& statsDesc);
