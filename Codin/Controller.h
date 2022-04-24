#pragma once

class Entity;

#include <iosfwd>
#include <memory>
#include <unordered_map>

class Controller
{
public:
	Controller(Entity* owner) : owner(owner) { }

	void Tick(const std::unordered_map<int, std::shared_ptr<Entity>>& allEntities);
	void MakeMove(std::ostream& out) const;

private:
	Entity* owner{};
};
