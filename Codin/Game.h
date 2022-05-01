#pragma once

#include "Vector.h"

#include <iosfwd>
#include <memory>
#include <unordered_map>
#include <vector>

class Entity;

struct EntityDescription;
struct StatsDescription;

enum class ControllerSetup : int8_t
{
	Default,
	Protect,
	Attack
};

class Game
{
public:
	Game(const Vector& basePosition, int numHeroes)
		: basePosition(basePosition), numHeroes(numHeroes)
	{ }
	Game(const Game&) = delete;

	int GetMana() const { return mana; }

	void Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc);

	void MakeMove(std::ostream& out) const;

	const std::unordered_map<int, std::shared_ptr<Entity>>& GetAllEntities() const { return allEntities; }
	const std::vector<std::shared_ptr<Entity>>& GetMyHeroes() const { return myHeroes; }
	const Vector& GetBasePosition() const { return basePosition; }
	Vector GetOpponentsBasePosition() const;

private:
	void TickAttackAndDefend();

	void ControllCreatedHero(Entity* hero);
	void SwitchControllersToProtectMyBase();
	void SwitchControllersToAttackOpponentsBase();
	bool ShouldAttack(const std::vector<Entity*> heroes) const;

	bool IsOpponentNearMyBase() const;

	std::vector<const Entity*> GetDangerousEnemies() const;
	std::vector<const Entity*> GetDangerousOpponents() const;
	std::vector<Entity*> GetHeroes();

	std::unordered_map<int, std::shared_ptr<Entity>> allEntities;
	std::vector<std::shared_ptr<Entity>> myHeroes;
	Vector basePosition{};
	int numHeroes{};
	int frame{};
	int health{};
	int mana{};
	ControllerSetup controllerSetup = ControllerSetup::Default;
};
