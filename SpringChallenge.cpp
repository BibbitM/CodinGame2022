#pragma region ..\Codin\Entity.cpp
// #include "Entity.h"
#pragma region ..\Codin\Entity.h
// #pragma once
// #include "Controller.h"
#pragma region ..\Codin\Controller.h
// #pragma once
#include <iosfwd>

class Entity;
class Game;

class Controller
{
public:
	Controller(Entity* owner) : owner(owner) { }
	virtual ~Controller() { }

	virtual void Tick(const Game& game) = 0;
	virtual void MakeMove(std::ostream& out) const = 0;

protected:
	Entity* owner{};
};
#pragma endregion ..\Codin\Controller.h
// #include "Vector.h"
#pragma region ..\Codin\Vector.h
// #pragma once
#include <iosfwd>

struct Vector
{
	int x, y;

	bool operator==(const Vector& b) const { return x == b.x && y == b.y; }

	Vector operator+(const Vector& b) const { return Vector{ x + b.x, y + b.y }; }
	Vector& operator+=(const Vector& b) { x += b.x; y += b.y; return *this; }

	Vector operator-(const Vector& b) const { return Vector{ x - b.x, y - b.y }; }
	Vector& operator-=(const Vector& b) { x -= b.x; y -= b.y; return *this; }

	Vector operator*(int b) const { return Vector{ x * b, y * b }; }
	Vector& operator*=(int b) { x *= b; y *= b; return *this; }

	Vector operator/(int b) const { return Vector{ x / b, y / b }; }
	Vector& operator/=(int b) { x /= b; y /= b; return *this; }
};

inline int Sqr(int val)
{
	return val * val;
}

inline int DistanceSqr(const Vector& a, const Vector& b)
{
	return Sqr(a.x - b.x) + Sqr(a.y - b.y);
}

std::istream& operator>>(std::istream& in, Vector& vec);
std::ostream& operator<<(std::ostream& out, const Vector& vec);
#pragma endregion ..\Codin\Vector.h

#include <cstdint>
#include <memory>

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

	void SetController(std::unique_ptr<Controller> controller);
	Controller* GetController() { return controllingBrain.get(); }

	const Vector& GetPosition() const { return position; }
	const Vector& GetVelocity() const { return velocity; }
	Vector GetTargetPosition() const { return position + velocity; }

	EntityType GetType() const { return type; }
	ThreatFor GetThreatFor() const { return threatFor; }
	int GetLastFrame() const { return lastFrame; }

private:
	std::unique_ptr<Controller> controllingBrain{};

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
#pragma endregion ..\Codin\Entity.h

// #include "EntityDescription.h"
#pragma region ..\Codin\EntityDescription.h
// #pragma once
// #include "Vector.h"

#include <iosfwd>

struct EntityDescription
{
	int id; // Unique identifier
	int type; // 0=monster, 1=your hero, 2=opponent hero
	Vector pos; // Position of this entity
	int shieldLife; // Ignore for this league; Count down until shield spell fades
	int isControlled; // Ignore for this league; Equals 1 when this entity is under a control spell
	int health; // Remaining health of this monster
	Vector vel; // Trajectory of this monster
	int nearBase; // 0=monster with no target yet, 1=monster targeting a base
	int threatFor; // Given this monster's trajectory, is it a threat to 1=your base, 2=your opponent's base, 0=neither
};

std::istream& operator>>(std::istream& in, EntityDescription& entDesc);
#pragma endregion ..\Codin\EntityDescription.h

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
#pragma endregion ..\Codin\Entity.cpp
#pragma region ..\Codin\EntityDescription.cpp
// #include "EntityDescription.h"

#include <iostream>

std::istream& operator>>(std::istream& in, EntityDescription& entDesc)
{
	in >> entDesc.id >> entDesc.type >> entDesc.pos >> entDesc.shieldLife >> entDesc.isControlled >> entDesc.health >> entDesc.vel >> entDesc.nearBase >> entDesc.threatFor;
	return in;
}
#pragma endregion ..\Codin\EntityDescription.cpp
#pragma region ..\Codin\Game.cpp
// #include "Game.h"
#pragma region ..\Codin\Game.h
// #pragma once
// #include "Vector.h"

#include <iosfwd>
#include <memory>
#include <unordered_map>
#include <vector>

class Entity;

struct EntityDescription;
struct StatsDescription;

class Game
{
public:
	Game(const Vector& basePosition, int numHeroes)
		: basePosition(basePosition), numHeroes(numHeroes)
	{ }
	Game(const Game&) = delete;

	void Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc);

	void MakeMove(std::ostream& out) const;

	const std::unordered_map<int, std::shared_ptr<Entity>>& GetAllEntities() const { return allEntities; }
	const Vector& GetBasePosition() const { return basePosition; }

private:
	void PossesEntity(Entity* hero);

	std::unordered_map<int, std::shared_ptr<Entity>> allEntities;
	std::vector<std::shared_ptr<Entity>> myHeroes;
	Vector basePosition{};
	int numHeroes{};
	int frame{};
};
#pragma endregion ..\Codin\Game.h

// #include "PaladinController.h"
#pragma region ..\Codin\PaladinController.h
// #pragma once
// #include "Controller.h"

class PaladinController : public Controller
{
public:
	PaladinController(Entity* owner) : Controller(owner) { }

	virtual void Tick(const Game& game) override;
	virtual void MakeMove(std::ostream& out) const override;

};

#pragma endregion ..\Codin\PaladinController.h
// #include "PeasantController.h"
#pragma region ..\Codin\PeasantController.h
// #pragma once
// #include "Controller.h"

// #include "Vector.h"

#include <iosfwd>

class PeasantController : public Controller
{
public:
	PeasantController(Entity* owner) : Controller(owner) { }

	virtual void Tick(const Game& game) override;
	virtual void MakeMove(std::ostream& out) const override;

protected:
	Vector targetPosition{};
};
#pragma endregion ..\Codin\PeasantController.h
// #include "Entity.h"
// #include "EntityDescription.h"
// #include "Rules.h"
#pragma region ..\Codin\Rules.h
// #pragma once
// #include "Vector.h"

struct Rules
{
	static constexpr Vector mapSize{ 17630, 9000 };

	static constexpr int baseViewRange = 6000;

	static constexpr int heroViewRange = 2200;
	static constexpr int heroMoveRange = 800;
	static constexpr int heroAttackRange = 800;
	static constexpr int heroDamage = 2;

	static constexpr int monsterMoveRange = 400;
	static constexpr int monsterBaseAttackRange = 5000;
	static constexpr int monsterBaseDestroyRange = 300;

	static constexpr int spellManaCost = 10;
	static constexpr int spellWindPushRange = 2200;
	static constexpr int spellWindRange = 1280;
	static constexpr int spellShieldTime = 12;
	static constexpr int spellShieldRange = 2200;
	static constexpr int spellControlRange = 2200;
};
#pragma endregion ..\Codin\Rules.h
// #include "Utils.h"
#pragma region ..\Codin\Utils.h
// #pragma once
#define UNUSED(x) (void)(x)
#pragma endregion ..\Codin\Utils.h

#include <algorithm>
#include <cassert>
#include <iostream>

void Game::Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc)
{
	UNUSED(myStats);
	UNUSED(opponentsStats);

	++frame;

	// Actualize all entities.
	for (const auto& entDesc : entitiesDesc)
	{
		auto entIt = allEntities.find(entDesc.id);
		if (entIt != allEntities.end())
			entIt->second->Actualize(entDesc, frame);
		else
		{
			entIt = allEntities.insert(std::make_pair(entDesc.id, std::make_shared<Entity>(entDesc, frame))).first;
			if (entIt->second->GetType() == EntityType::MyHero)
			{
				PossesEntity(entIt->second.get());
				myHeroes.push_back(entIt->second);
			}
		}
	}

	// Remove no longer valid entities.
	for (auto it = allEntities.begin(); it != allEntities.end(); /* inside the loop */)
	{
		if (it->second->GetLastFrame() != frame)
		{
			assert(it->second->GetType() != EntityType::MyHero);
			it = allEntities.erase(it);
		}
		else
			++it;
	}

	// Tick all heroes.
	for (const auto& hero : myHeroes)
		hero->GetController()->Tick(*this);
}

void Game::MakeMove(std::ostream& out) const
{
	for (const auto& hero : myHeroes)
		hero->GetController()->MakeMove(out);
}

void Game::PossesEntity(Entity* hero)
{
	std::unique_ptr<Controller> controller{};
	if (myHeroes.empty())
		controller = std::make_unique<PaladinController>(hero);
	else
		controller = std::make_unique<PeasantController>(hero);

	hero->SetController(std::move(controller));
}
#pragma endregion ..\Codin\Game.cpp
#pragma region ..\Codin\PaladinController.cpp
// #include "PaladinController.h"

// #include "Utils.h"

#include <iostream>

void PaladinController::Tick(const Game& game)
{
	UNUSED(game);
}

void PaladinController::MakeMove(std::ostream& out) const
{
	out << "WAIT";

	out << " Paladin" << std::endl;
}
#pragma endregion ..\Codin\PaladinController.cpp
#pragma region ..\Codin\PeasantController.cpp
// #include "PeasantController.h"

// #include "Entity.h"
// #include "Game.h"

#include <algorithm>
#include <iostream>
#include <vector>

void PeasantController::Tick(const Game& game)
{
	std::vector<Entity*> myEnemies;
	myEnemies.reserve(game.GetAllEntities().size());
	std::vector<Entity*> otherEnemies;
	otherEnemies.reserve(game.GetAllEntities().size());

	for (const auto& ent : game.GetAllEntities())
	{
		if (ent.second->GetThreatFor() == ThreatFor::MyBase)
			myEnemies.push_back(ent.second.get());
		else if (ent.second->GetType() == EntityType::Monster)
			otherEnemies.push_back(ent.second.get());
	}

	auto compareEnemyDist = [&game](Entity* a, Entity* b) { return DistanceSqr(a->GetTargetPosition(), game.GetBasePosition()) < DistanceSqr(b->GetTargetPosition(), game.GetBasePosition()); };

	if (!myEnemies.empty())
	{
		std::sort(myEnemies.begin(), myEnemies.end(), compareEnemyDist);
		targetPosition = myEnemies.front()->GetTargetPosition();
	}
	else if (!otherEnemies.empty())
	{
		std::sort(otherEnemies.begin(), otherEnemies.end(), compareEnemyDist);
		targetPosition = otherEnemies.front()->GetTargetPosition();
	}
	else
		targetPosition = owner->GetPosition();
}

void PeasantController::MakeMove(std::ostream& out) const
{
	if (owner->GetPosition() == targetPosition)
		out << "WAIT";
	else
		out << "MOVE " << targetPosition;

	out << " Peasant" << std::endl;
}
#pragma endregion ..\Codin\PeasantController.cpp
#pragma region ..\Codin\StatsDescription.cpp
// #include "StatsDescription.h"
#pragma region ..\Codin\StatsDescription.h
// #pragma once
#include <iosfwd>

struct StatsDescription
{
	int health; // Your base health
	int mana; // Ignore in the first league; Spend ten mana to cast a spell
};

std::istream& operator>>(std::istream& in, StatsDescription& statsDesc);
#pragma endregion ..\Codin\StatsDescription.h

#include <iostream>

std::istream& operator>>(std::istream& in, StatsDescription& statsDesc)
{
	in >> statsDesc.health >> statsDesc.mana;
	return in;
}
#pragma endregion ..\Codin\StatsDescription.cpp
#pragma region ..\Codin\Vector.cpp
// #include "Vector.h"

#include <iostream>

std::istream& operator>>(std::istream& in, Vector& vec)
{
	in >> vec.x >> vec.y;
	return in;
}

std::ostream& operator<<(std::ostream& out, const Vector& vec)
{
	out << vec.x << ' ' << vec.y;
	return out;
}
#pragma endregion ..\Codin\Vector.cpp
#pragma region Main.cpp
// #include "../Codin/EntityDescription.h"
// #include "../Codin/Game.h"
// #include "../Codin/StatsDescription.h"
// #include "../Codin/Vector.h"

#include <iostream>
#include <vector>

int main()
{
	Vector base; // The corner of the map representing your base
	std::cin >> base; std::cin.ignore();
	int heroesPerPlayer; // Always 3
	std::cin >> heroesPerPlayer; std::cin.ignore();

	Game game(base, heroesPerPlayer);

	// game loop
	while (1)
	{
		StatsDescription myStatsDesc;
		std::cin >> myStatsDesc; std::cin.ignore();
		StatsDescription opponentStatsDesc;
		std::cin >> opponentStatsDesc; std::cin.ignore();

		int entitiesCount; // Amount of heroes and monsters you can see
		std::cin >> entitiesCount; std::cin.ignore();
		std::vector<EntityDescription> entitesDesc;
		entitesDesc.reserve(entitiesCount);
		for (int i = 0; i < entitiesCount; i++)
		{
			std::cin >> entitesDesc.emplace_back(); std::cin.ignore();
		}

		game.Tick(myStatsDesc, opponentStatsDesc, entitesDesc);
		game.MakeMove(std::cout);
	}
}
#pragma endregion Main.cpp
