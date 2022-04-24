// ..\Codin\Controller.cpp
// #include "Controller.h" begin
// ..\Codin\Controller.h
// #pragma once
// #include "Vector.h" begin
// ..\Codin\Vector.h
// #pragma once
#include <cmath>
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
// #include "Vector.h" end

#include <iosfwd>
#include <memory>
#include <unordered_map>

class Entity;
class Game;

class Controller
{
public:
	Controller(Entity* owner) : owner(owner) { }

	void Tick(const Game& game);
	void MakeMove(std::ostream& out) const;

private:
	Entity* owner{};
	Vector targetPosition{};
};
// #include "Controller.h" end

// #include "Entity.h" begin
// ..\Codin\Entity.h
// #pragma once
// #include "Controller.h" begin
// #include "Controller.h" end
// #include "Vector.h" begin
// #include "Vector.h" end

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
// #include "Entity.h" end
// #include "Game.h" begin
// ..\Codin\Game.h
// #pragma once
// #include "Vector.h" begin
// #include "Vector.h" end

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
	Game(int numHeroes)
		: numHeroes(numHeroes)
	{ }
	Game(const Game&) = delete;

	void Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc);

	void MakeMove(std::ostream& out) const;

	const std::unordered_map<int, std::shared_ptr<Entity>>& GetAllEntities() const { return allEntities; }
	const Vector& GetBasePosition() const { return basePosition; }

private:
	std::unordered_map<int, std::shared_ptr<Entity>> allEntities;
	std::vector<std::shared_ptr<Entity>> myHeroes;
	Vector basePosition{};
	int numHeroes{};
	int frame{};
};
// #include "Game.h" end

#include <algorithm>
#include <iostream>
#include <vector>

void Controller::Tick(const Game& game)
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

void Controller::MakeMove(std::ostream& out) const
{
	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	// In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;

	if (owner->GetPosition() == targetPosition)
		out << "WAIT" << std::endl;
	else
		out << "MOVE " << targetPosition << std::endl;
}
// ..\Codin\Entity.cpp
// #include "Entity.h" begin
// #include "Entity.h" end

// #include "EntityDescription.h" begin
// ..\Codin\EntityDescription.h
// #pragma once
// #include "Vector.h" begin
// #include "Vector.h" end

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
// #include "EntityDescription.h" end

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
// ..\Codin\EntityDescription.cpp
// #include "EntityDescription.h" begin
// #include "EntityDescription.h" end

#include <iostream>

std::istream& operator>>(std::istream& in, EntityDescription& entDesc)
{
	in >> entDesc.id >> entDesc.type >> entDesc.pos >> entDesc.shieldLife >> entDesc.isControlled >> entDesc.health >> entDesc.vel >> entDesc.nearBase >> entDesc.threatFor;
	return in;
}
// ..\Codin\Game.cpp
// #include "Game.h" begin
// #include "Game.h" end

// #include "Controller.h" begin
// #include "Controller.h" end
// #include "Entity.h" begin
// #include "Entity.h" end
// #include "EntityDescription.h" begin
// #include "EntityDescription.h" end
// #include "Rules.h" begin
// ..\Codin\Rules.h
// #pragma once
// #include "Vector.h" begin
// #include "Vector.h" end

struct Rules
{
	static constexpr Vector mapSize{ 17630, 9000 };
};
// #include "Rules.h" end
// #include "Utils.h" begin
// ..\Codin\Utils.h
// #pragma once
#define UNUSED(x) (void)(x)
// #include "Utils.h" end

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
				entIt->second->SetController(std::make_unique<Controller>(entIt->second.get()));
				myHeroes.push_back(entIt->second);

				// Determine base position
				if (frame == 1)
				{
					const Vector& heroPosition = entIt->second->GetPosition();
					if (DistanceSqr(heroPosition, Rules::mapSize) < DistanceSqr(heroPosition, Vector{}))
						basePosition = Rules::mapSize;
				}
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
// ..\Codin\StatsDescription.cpp
// #include "StatsDescription.h" begin
// ..\Codin\StatsDescription.h
// #pragma once
#include <iosfwd>

struct StatsDescription
{
	int health; // Your base health
	int mana; // Ignore in the first league; Spend ten mana to cast a spell
};

std::istream& operator>>(std::istream& in, StatsDescription& statsDesc);

// #include "StatsDescription.h" end

#include <iostream>

std::istream& operator>>(std::istream& in, StatsDescription& statsDesc)
{
	in >> statsDesc.health >> statsDesc.mana;
	return in;
}
// ..\Codin\Vector.cpp
// #include "Vector.h" begin
// #include "Vector.h" end

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
// Main.cpp
// #include "../Codin/EntityDescription.h" begin
// #include "../Codin/EntityDescription.h" end
// #include "../Codin/Game.h" begin
// #include "../Codin/Game.h" end
// #include "../Codin/StatsDescription.h" begin
// #include "../Codin/StatsDescription.h" end
// #include "../Codin/Vector.h" begin
// #include "../Codin/Vector.h" end

#include <iostream>
#include <vector>

int main()
{
	Vector base; // The corner of the map representing your base
	std::cin >> base; std::cin.ignore();
	int heroesPerPlayer; // Always 3
	std::cin >> heroesPerPlayer; std::cin.ignore();

	Game game(heroesPerPlayer);

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
