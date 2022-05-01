#pragma region ..\Codin\Controller.cpp
// #include "Controller.h"
#pragma region ..\Codin\Controller.h
// #pragma once
// #include "Vector.h"
#pragma region ..\Codin\Vector.h
// #pragma once
// #include "Math.h"
#pragma region ..\Codin\Math.h
// #pragma once
#include <cmath>

constexpr inline int Pow2(int val)
{
	return val * val;
}

inline int Sqrt(int val)
{
	return static_cast<int>(std::sqrt(static_cast<float>(val)));
}
#pragma endregion ..\Codin\Math.h

#include <iosfwd>

struct Vector
{
	int x, y;

	bool operator==(const Vector& b) const { return x == b.x && y == b.y; }
	bool operator!=(const Vector& b) const { return !(*this == b); }

	Vector operator+(const Vector& b) const { return { x + b.x, y + b.y }; }
	Vector& operator+=(const Vector& b) { x += b.x; y += b.y; return *this; }

	Vector operator-(const Vector& b) const { return { x - b.x, y - b.y }; }
	Vector& operator-=(const Vector& b) { x -= b.x; y -= b.y; return *this; }

	Vector operator*(int b) const { return { x * b, y * b }; }
	Vector& operator*=(int b) { x *= b; y *= b; return *this; }

	Vector operator/(int b) const { return { x / b, y / b }; }
	Vector& operator/=(int b) { x /= b; y /= b; return *this; }

	Vector operator-() const { return { -x, -y }; };

	constexpr int Length2() const { return Pow2(x) + Pow2(y); }
	Vector Lengthed(int length) const;
	Vector Limited(int lenght) const;
};

inline int Distance2(const Vector& a, const Vector& b)
{
	return Pow2(a.x - b.x) + Pow2(a.y - b.y);
}

std::istream& operator>>(std::istream& in, Vector& vec);
std::ostream& operator<<(std::ostream& out, const Vector& vec);
#pragma endregion ..\Codin\Vector.h

#include <iosfwd>
#include <string>
#include <string_view>

class Entity;
class Game;

enum class Spell : int8_t
{
	None,
	Wind,
	Shield,
	Control,
};

std::ostream& operator<<(std::ostream& out, Spell spell);

class Controller
{
public:
	Controller(const Entity& owner, std::string_view name) : owner(owner), name(name) { }
	virtual ~Controller() { }

	void Clear();
	virtual bool Attack(const Game& game, const Entity& danger) = 0;;
	virtual void Tick(const Game& game) = 0;
	void MakeMove(std::ostream& out) const;

protected:
	void SetTarget(int entity, const Vector& pos, std::string_view info);
	void SetSpell(Spell spell, int entity, const Vector& pos, std::string_view info);

	bool HasTarget() const { return targetEntity != -1; }
	static bool IsTargetedEntity(int entity, const Game& game);

	const Entity& owner;

private:
	const std::string name{};
	Vector targetPosition{};
	int targetEntity{ -1 };
	Spell targetSpell{ Spell::None };
};
#pragma endregion ..\Codin\Controller.h

// #include "Entity.h"
#pragma region ..\Codin\Entity.h
// #pragma once
// #include "Controller.h"
// #include "Vector.h"

#include <cstdint>
#include <memory>

struct EntityDescription;

enum class EntityType : int8_t
{
	Monster,
	MyHero,
	OpponentsHero,
};

enum class ThreatFor : int8_t
{
	None,
	MyBase,
	OpponentsBase,
};

class Entity
{
public:
	Entity(const EntityDescription& entityDesc, int frame);
	Entity(const Entity& entity);
	void Actualize(const EntityDescription& entityDesc, int frame);

	void SetController(std::unique_ptr<Controller> controller);
	Controller* GetController() { return controllingBrain.get(); }

	int GetId() const { return id; }
	const Vector& GetPosition() const { return position; }
	const Vector& GetVelocity() const { return velocity; }
	Vector GetTargetPosition() const { return position + velocity; }
	EntityType GetType() const { return type; }
	ThreatFor GetThreatFor() const { return threatFor; }
	int GetShieldLife() const { return shieldLife; }
	int GetHealt() const { return health; }
	bool IsControlled() const { return isControlled; }
	bool IsNearBase() const { return isNearBase; }

	int GetLastFrame() const { return lastFrame; }

	void SetPosition(const Vector& pos) { position = pos; }
	void SetVelocity(const Vector& vel) { velocity = vel; }
	void SetNearBase(bool isNear) { isNearBase = isNear; }

	Vector GetAwayDirection(const Entity& other) const;

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

	int GetMana() const { return mana; }

	void Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc);

	void MakeMove(std::ostream& out) const;

	const std::unordered_map<int, std::shared_ptr<Entity>>& GetAllEntities() const { return allEntities; }
	const std::vector<std::shared_ptr<Entity>>& GetMyHeroes() const { return myHeroes; }
	const Vector& GetBasePosition() const { return basePosition; }
	Vector GetOpponentsBasePosition() const;

private:
	void PossesEntity(Entity* hero);

	std::vector<const Entity*> GetDangerousEnemies() const;
	std::vector<Entity*> GetHeroes();

	std::unordered_map<int, std::shared_ptr<Entity>> allEntities;
	std::vector<std::shared_ptr<Entity>> myHeroes;
	Vector basePosition{};
	int numHeroes{};
	int frame{};
	int health{};
	int mana{};
};
#pragma endregion ..\Codin\Game.h
// #include "Utils.h"
#pragma region ..\Codin\Utils.h
// #pragma once
/// Mark that given parameter is not used at all in the function.
#define UNUSED(x) (void)(x)
/// Mark that given parameter is not used in given code path.
#define TOUCH(x) (void)(x)

// https://stackoverflow.com/questions/1082192/how-to-generate-random-variable-names-in-c-using-macros
// One level of macro indirection is required in order to resolve __COUNTER__,
// and get varname1 instead of varname__COUNTER__.
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define UNIQUE_NAME(base) CONCAT(base, __COUNTER__)
#pragma endregion ..\Codin\Utils.h

#include <iostream>

#define LOG_TARGET 1

void Controller::Clear()
{
	targetPosition = owner.GetPosition();
	targetEntity = -1;
	targetSpell = Spell::None;
}

void Controller::MakeMove(std::ostream& out) const
{
	if (targetSpell != Spell::None)
	{
		out << "SPELL " << targetSpell;
		if (targetSpell == Spell::Shield || targetSpell == Spell::Control)
			out << ' ' << targetEntity;
		if (targetSpell == Spell::Wind || targetSpell == Spell::Control)
			out << ' ' << targetPosition;
	}
	else if (owner.GetPosition() != targetPosition)
		out << "MOVE " << targetPosition;
	else
		out << "WAIT";

	out << ' ' << name << owner.GetId() << std::endl;
}

void Controller::SetTarget(int entity, const Vector& pos, std::string_view info)
{
	targetEntity = entity;
	targetPosition = pos;
	targetSpell = Spell::None;

#if LOG_TARGET
	std::cerr << "H(" << owner.GetId() << ") Move -> T:" << entity << " P:" << pos << ' ' << info << std::endl;
#else
	TOUCH(info);
#endif
}

void Controller::SetSpell(Spell spell, int entity, const Vector& pos, std::string_view info)
{
	targetEntity = entity;
	targetPosition = pos;
	targetSpell = spell;

#if LOG_TARGET
	std::cerr << "H(" << owner.GetId() << ") S(" << spell << ") ->" << " T:" << entity << " P:" << pos << ' ' << info << std::endl;
#else
	TOUCH(info);
#endif

}

bool Controller::IsTargetedEntity(int entity, const Game& game)
{
	for (auto hero : game.GetMyHeroes())
	{
		if (hero->GetController()->targetEntity == entity)
			return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream& out, Spell spell)
{
	std::string_view spellName{};
	switch (spell)
	{
	case Spell::Wind: spellName = "WIND"; break;
	case Spell::Shield: spellName = "SHIELD"; break;
	case Spell::Control: spellName = "CONTROL"; break;
	}
	return out << spellName;
}
#pragma endregion ..\Codin\Controller.cpp
#pragma region ..\Codin\Entity.cpp
// #include "Entity.h"

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
std::ostream& operator<<(std::ostream& out, const EntityDescription& entDesc);
#pragma endregion ..\Codin\EntityDescription.h

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

Vector Entity::GetAwayDirection(const Entity& other) const
{
	if (GetPosition() != other.GetPosition())
		return GetPosition() - other.GetPosition();

	Vector awayDirection{
		(id < other.id) ? -1 : 1,
		(id % 2 < other.id % 2) ? -1 : 1,
	};
	return awayDirection;
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

std::ostream& operator<<(std::ostream& out, const EntityDescription& entDesc)
{
	out << entDesc.id << ' ' << entDesc.type << ' ' << entDesc.pos << ' ' << entDesc.shieldLife << ' ' << entDesc.isControlled << ' ' << entDesc.health << ' ' << entDesc.vel << ' ' << entDesc.nearBase << ' ' << entDesc.threatFor;
	return out;
}
#pragma endregion ..\Codin\EntityDescription.cpp
#pragma region ..\Codin\Game.cpp
// #include "Game.h"

// #include "PaladinController.h"
#pragma region ..\Codin\PaladinController.h
// #pragma once
// #include "Controller.h"
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

class PaladinController : public Controller
{
public:
	PaladinController(const Entity& owner) : Controller(owner, "Paladin") {}

	virtual bool Attack(const Game& game, const Entity& danger) override;
	virtual void Tick(const Game& game) override;

private:
	bool TryCastSpellOnNearestOpponent(const Game& game);
	bool TryGainMaxWildMana(const Game& game);

	Vector GetIdleTarget(const Game& game) const;

	bool wantsMoveCloserToBase = false;

	static constexpr int minDistToBase = Rules::baseViewRange + Rules::heroViewRange * 1 / 4; // 1 / 2;
	static constexpr int maxDistToBase = Rules::baseViewRange + Rules::heroViewRange * 2 / 2; // 7 / 10; // 7/10 ~= Sqrt(2) / 2 ~= 0.707107
	static constexpr int minDistToEdge = Rules::heroViewRange * 7 / 10;
	static constexpr int minDistToHero = 2 * Rules::heroViewRange * 7 / 10;
	static constexpr int sensDistToEnemy = Rules::heroViewRange * 3 / 2;
	static constexpr int optDistToEnemy = Rules::heroAttackRange * 1 / 2;
};

#pragma endregion ..\Codin\PaladinController.h
// #include "PeasantController.h"
#pragma region ..\Codin\PeasantController.h
// #pragma once
// #include "Controller.h"

class PeasantController : public Controller
{
public:
	PeasantController(const Entity& owner) : Controller(owner, "Peasant") {}

	virtual bool Attack(const Game& game, const Entity& danger) override;
	virtual void Tick(const Game& game) override;
};
#pragma endregion ..\Codin\PeasantController.h
// #include "Entity.h"
// #include "EntityDescription.h"
// #include "Rules.h"
// #include "Simulate.h"
#pragma region ..\Codin\Simulate.h
// #pragma once
class Entity;
class Game;
struct Vector;

#include <vector>

class Simulate
{
public:
	static Vector GetNearestBasePosition(const Entity& entity);
	static void Update(Entity& entity);
	static Vector PositionAfterFrames(const Entity& entity, int frames);
	static int EnemyFramesToAttackBase(const Entity& entity);
	static int HeroFramesToAttackEnemy(const Entity& hero, const Entity& enemy);
	static int HeroFramesToCastSpell(const Entity& hero, const Entity& enemy, int spellRange);
	static int FramesToKill(int healt);

	// Find the attack position damaging the most enemies at once or closest to the preferred position in attack and move range.
	static Vector GetBestAttackPosition(const Entity& hero, const Entity& danger, const Vector& preferedPosition, const Game& game);
	// Find the attack position closest to the preferred position in attack and move range.
	static Vector GetPreferedAttackPosition(const Entity& hero, const Entity& danger, const Vector& preferedPosition);

	static Vector GetEnemiesCenter(const std::vector<const Entity*>& enemies);
};
#pragma endregion ..\Codin\Simulate.h
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
// #include "Utils.h"

#include <algorithm>
#include <cassert>
#include <iostream>

void Game::Tick(const StatsDescription& myStats, const StatsDescription& opponentsStats, const std::vector<EntityDescription>& entitiesDesc)
{
	UNUSED(opponentsStats);

	++frame;

	health = myStats.health;
	mana = myStats.mana;

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

	// Prepare all heroes for new tick.
	for (const auto& hero : myHeroes)
		hero->GetController()->Clear();

	// First take care of dangerous enemies
	std::vector<const Entity*> dangerousEnemies = GetDangerousEnemies();
	std::vector<Entity*> heroes = GetHeroes();
	for (const Entity* danger : dangerousEnemies)
	{
		std::sort(heroes.begin(), heroes.end(), [danger](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetPosition(), danger->GetPosition()) < Distance2(b->GetPosition(), danger->GetPosition());
		});

		for (auto it = heroes.begin(); it != heroes.end(); /* in loop */)
		{
			if ((*it)->GetController()->Attack(*this, *danger))
			{
				const int dangerFrameToAttackBase = Simulate::EnemyFramesToAttackBase(*danger);
				const int heroFrameToAttackDanger = Simulate::HeroFramesToAttackEnemy(*(*it), *danger);
				const int heroFrameToKill = Simulate::FramesToKill(danger->GetHealt());

				it = heroes.erase(it);

				// If one hero can deal with it attack next danger.
				if (danger->GetShieldLife() == 0
					|| heroFrameToKill + heroFrameToAttackDanger <= dangerFrameToAttackBase)
				{
					break;
				}
			}
			else
				++it;
		}
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

Vector Game::GetOpponentsBasePosition() const
{
	return basePosition == Vector{} ? Rules::mapSize : Vector{};
}

void Game::PossesEntity(Entity* hero)
{
	std::unique_ptr<Controller> controller{};
	//if (myHeroes.empty())
	//	controller = std::make_unique<PeasantController>(*hero);
	//else
		controller = std::make_unique<PaladinController>(*hero);

	hero->SetController(std::move(controller));
}

std::vector<const Entity*> Game::GetDangerousEnemies() const
{
	std::vector<const Entity*> dangerousEnemies;
	dangerousEnemies.reserve(GetAllEntities().size());

	// Get only enemies.
	for (const auto& ent : GetAllEntities())
	{
		const auto& enemy = ent.second;
		if (enemy->GetThreatFor() == ThreatFor::MyBase)
			dangerousEnemies.push_back(enemy.get());
	}

	// Sort enemies by danger to the base.
	std::sort(dangerousEnemies.begin(), dangerousEnemies.end(), [this](const Entity* a, const Entity* b)
	{
		const int aFTDD = Simulate::EnemyFramesToAttackBase(*a);
		const int bFTDD = Simulate::EnemyFramesToAttackBase(*b);
		if (aFTDD < bFTDD)
			return true;
		if (aFTDD > bFTDD)
			return false;

		const int aDist2 = Distance2(a->GetPosition(), GetBasePosition());
		const int bDist2 = Distance2(b->GetPosition(), GetBasePosition());
		return aDist2 < bDist2;
	});

	return dangerousEnemies;
}

std::vector<Entity*> Game::GetHeroes()
{
	std::vector<Entity*> heroes;
	heroes.reserve(myHeroes.size());

	for (const std::shared_ptr<Entity>& hero : myHeroes)
		heroes.push_back(hero.get());

	return heroes;
}
#pragma endregion ..\Codin\Game.cpp
#pragma region ..\Codin\PaladinController.cpp
// #include "PaladinController.h"

// #include "Entity.h"
// #include "Game.h"
// #include "Math.h"
// #include "Simulate.h"
// #include "Timer.h"
#pragma region ..\Codin\Timer.h
// #pragma once
// #include "Utils.h"

#include <chrono>
#include <iostream>
#include <string_view>

#define MEASURE_EXECUTION_TIME 0

// https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c

class MeasureTimer
{
public:
	MeasureTimer(std::string_view scopeName)
		: start(std::chrono::high_resolution_clock::now())
		, functionName(scopeName)
	{}

	~MeasureTimer()
	{
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::cerr << functionName << ' ' << std::chrono::duration<float, std::milli>(end - start).count() << "ms" << std::endl;
	}

private:
	std::chrono::high_resolution_clock::time_point start;
	std::string_view functionName;
};

#if MEASURE_EXECUTION_TIME
#define FUNCTION_TIMER() MeasureTimer UNIQUE_NAME(functionTimer){ __FUNCTION__ }
#define SCOPE_TIMER(scopeName) MeasureTimer UNIQUE_NAME(scopeTimer){ scopeName }
#else
#define FUNCTION_TIMER() (void)(0)
#define SCOPE_TIMER() (void)(0)
#endif
#pragma endregion ..\Codin\Timer.h

#include <algorithm>
#include <iostream>
#include <vector>

#define LOG_PALADIN_CONTROLLER 1

bool PaladinController::Attack(const Game& game, const Entity& danger)
{
	const int dangerFrameToAttackBase = Simulate::EnemyFramesToAttackBase(danger);
	const int heroFrameToAttackDanger = Simulate::HeroFramesToAttackEnemy(owner, danger);
	const int heroFrameToCastWind = Simulate::HeroFramesToCastSpell(owner, danger, Rules::spellWindRange);
	const int heroFrameToKill = Simulate::FramesToKill(danger.GetHealt());

	constexpr int framesCanIgnore = 5;

	// I cannot reach danger before he destroy the base.
	// TODO: add spell check.
	if (dangerFrameToAttackBase < heroFrameToAttackDanger)
		return false;

	const bool heroNearBase = Distance2(owner.GetPosition(), game.GetBasePosition()) <= Pow2(Rules::monsterBaseAttackRange);
	const bool dangerNearBase = Distance2(danger.GetTargetPosition(), game.GetBasePosition()) <= Pow2(Rules::monsterBaseAttackRange);

	// Check if I have time to attack other enemies.
	if (!heroNearBase && !dangerNearBase
		&& dangerFrameToAttackBase > heroFrameToAttackDanger + heroFrameToKill + framesCanIgnore)
	{
		return false;
	}

#if LOG_PALADIN_CONTROLLER
	std::cerr
		<< " FB:" << dangerFrameToAttackBase
		<< " FA:" << heroFrameToAttackDanger
		<< " FK:" << heroFrameToKill
		<< " FC:" << heroFrameToCastWind
		<< std::endl;
#endif

	// Check if I have to cast spell
	if (danger.GetShieldLife() == 0
		&& dangerFrameToAttackBase <= 2
		&& heroFrameToCastWind == 0
		&& dangerFrameToAttackBase <= heroFrameToAttackDanger + heroFrameToKill
		&& game.GetMana() > Rules::spellManaCost)
	{
		SetSpell(Spell::Wind, danger.GetId(), game.GetOpponentsBasePosition(), "PC-attackWind");
		return true;
	}

	// I've to attack.
	Vector attackPosition = heroFrameToAttackDanger > 0
		? Simulate::PositionAfterFrames(danger, heroFrameToAttackDanger)
		: Simulate::GetBestAttackPosition(owner, danger, game.GetBasePosition(), game);
	SetTarget(danger.GetId(), attackPosition, "PC-attack");
	return true;
}

void PaladinController::Tick(const Game& game)
{
	FUNCTION_TIMER();

	if (wantsMoveCloserToBase && Distance2(owner.GetPosition(), game.GetBasePosition()) <= Pow2(minDistToBase))
		wantsMoveCloserToBase = false;
	else if (!wantsMoveCloserToBase && Distance2(owner.GetPosition(), game.GetBasePosition()) >= Pow2(maxDistToBase))
		wantsMoveCloserToBase = true;

	// We have already attacked dangerous enemy. No other move is needed.
	if (HasTarget())
		return;

	if (TryCastSpellOnNearestOpponent(game))
		return;

	if (TryGainMaxWildMana(game))
		return;

	// No enemies to attack, 
	SetTarget(-1, GetIdleTarget(game), "PC-idle");
}

bool PaladinController::TryCastSpellOnNearestOpponent(const Game& game)
{
	// Check if we have enough mana.
	if (game.GetMana() < Rules::spellManaCost * 3)
		return false;

	std::vector<const Entity*> opponents;
	opponents.reserve(3);

	bool tryToCastShield = false;

	// Get opponents in spell range.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() != EntityType::OpponentsHero)
			continue;

		const int opponentDist2 = Distance2(enemy->GetPosition(), owner.GetPosition());
		if (opponentDist2 <= Pow2(Rules::spellControlRange * 2))
			tryToCastShield = true;
		
		if (opponentDist2 <= Pow2(Rules::spellControlRange))
			opponents.push_back(enemy);
	}

	if (tryToCastShield && owner.GetShieldLife() == 0)
	{
		SetSpell(Spell::Shield, owner.GetId(), owner.GetPosition(), "PC-spellShield");
		return true;
	}

	if (!opponents.empty())
	{
		std::sort(opponents.begin(), opponents.end(), [this](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetPosition(), owner.GetPosition()) < Distance2(b->GetPosition(), owner.GetPosition());
		});

		for (const Entity* opponent : opponents)
		{
			if (opponent->GetShieldLife() == 0 &&
				!IsTargetedEntity(opponent->GetId(), game))
			{
				SetSpell(Spell::Control, opponent->GetId(), game.GetOpponentsBasePosition(), "PC-spellControl");
				return true;
			}
		}
	}

	return false;
}

bool PaladinController::TryGainMaxWildMana(const Game& game)
{
	std::vector<const Entity*> enemies;
	enemies.reserve(game.GetAllEntities().size());

	// Get enemies in one move range that will score extra mana.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() == EntityType::Monster
			&& Distance2(enemy->GetPosition(), game.GetBasePosition()) > Pow2(Rules::baseViewRange)
			&& Simulate::HeroFramesToAttackEnemy(owner, *enemy) == 0)
		{
			enemies.push_back(enemy);
		}
	}

	if (!enemies.empty())
	{
		std::sort(enemies.begin(), enemies.end(), [this](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetPosition(), owner.GetPosition()) < Distance2(b->GetPosition(), owner.GetPosition());
		});

		const Entity* enemy = enemies.front();
		const Vector attackPosition = Simulate::GetBestAttackPosition(owner, *enemy, GetIdleTarget(game)/*enemy->GetTargetPosition()*/, game);
		SetTarget(enemy->GetId(), attackPosition, "PC-enemy0");
		return true;
	}

	enemies.clear();

	// Get enemies in two move range that will score extra mana.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() == EntityType::Monster
			&& Distance2(enemy->GetPosition(), game.GetBasePosition()) > Pow2(Rules::baseViewRange)
			&& Distance2(enemy->GetTargetPosition(), game.GetBasePosition()) > Pow2(Rules::baseViewRange)
			&& Simulate::HeroFramesToAttackEnemy(owner, *enemy) == 1)
		{
			enemies.push_back(enemy);
		}
	}

	if (!enemies.empty())
	{
		std::sort(enemies.begin(), enemies.end(), [this](const Entity* a, const Entity* b)
		{
			return Distance2(a->GetTargetPosition(), owner.GetPosition()) < Distance2(b->GetTargetPosition(), owner.GetPosition());
		});

		const Entity* enemy = enemies.front();
		SetTarget(enemy->GetId(), enemy->GetTargetPosition(), "PC-enemy1");
		return true;
	}

	return false;
}

Vector PaladinController::GetIdleTarget(const Game& game) const
{
	Vector idleTarget = owner.GetPosition();

	// Set minimal distance to the base.
	const int distToBase2 = Distance2(owner.GetPosition(), game.GetBasePosition());
	if (wantsMoveCloserToBase && distToBase2 > Pow2(minDistToBase))
		idleTarget += (game.GetBasePosition() - owner.GetPosition()).Lengthed(std::max(Sqrt(distToBase2) - minDistToBase, Rules::heroMoveRange));
	else if (!wantsMoveCloserToBase && distToBase2 < Pow2(maxDistToBase))
		idleTarget += (owner.GetPosition() - game.GetBasePosition()).Lengthed(std::max(maxDistToBase - Sqrt(distToBase2), Rules::heroMoveRange));

	// Set minimal distance to the map edges.
	if (owner.GetPosition().x < minDistToEdge)
		idleTarget.x += minDistToEdge - owner.GetPosition().x;
	if (owner.GetPosition().y < minDistToEdge)
		idleTarget.y += minDistToEdge - owner.GetPosition().y;
	if (Rules::mapSize.x - owner.GetPosition().x < minDistToEdge)
		idleTarget.x -= minDistToEdge - (Rules::mapSize.x - owner.GetPosition().x);
	if (Rules::mapSize.y - owner.GetPosition().y < minDistToEdge)
		idleTarget.y -= minDistToEdge - (Rules::mapSize.y - owner.GetPosition().y);

	// Set minimal distance to other players.
	for (const auto& hero : game.GetMyHeroes())
	{
		if (hero.get() == &owner)
			continue;

		const int distToHero2 = Distance2(owner.GetPosition(), hero->GetPosition());
		if (distToHero2 < Pow2(minDistToHero))
			idleTarget += owner.GetAwayDirection(*hero).Lengthed((minDistToHero - Sqrt(distToHero2)) / 2);
	}

	// Try to move to near enemies.
	for (const auto& ent : game.GetAllEntities())
	{
		const Entity* enemy = ent.second.get();
		if (enemy->GetType() != EntityType::Monster)
			continue;

		const int distToEnemy2 = Distance2(enemy->GetPosition(), owner.GetPosition());
		if (distToEnemy2 < Pow2(sensDistToEnemy) && distToEnemy2 > Pow2(optDistToEnemy))
			idleTarget += (enemy->GetPosition() - owner.GetPosition()).Lengthed(Sqrt(distToEnemy2) - optDistToEnemy);
	}

	return idleTarget;
}
#pragma endregion ..\Codin\PaladinController.cpp
#pragma region ..\Codin\PeasantController.cpp
// #include "PeasantController.h"

// #include "Entity.h"
// #include "Game.h"
// #include "Utils.h"

#include <algorithm>
#include <vector>

bool PeasantController::Attack(const Game& game, const Entity& danger)
{
	UNUSED(game);
	SetTarget(danger.GetId(), danger.GetTargetPosition(), "PE-attackDanger");
	return true;
}

void PeasantController::Tick(const Game& game)
{
	if (HasTarget())
		return;

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

	auto compareEnemyDist = [&game](Entity* a, Entity* b) { return Distance2(a->GetTargetPosition(), game.GetBasePosition()) < Distance2(b->GetTargetPosition(), game.GetBasePosition()); };

	if (!myEnemies.empty())
	{
		std::sort(myEnemies.begin(), myEnemies.end(), compareEnemyDist);
		SetTarget(myEnemies.front()->GetId(), myEnemies.front()->GetTargetPosition(), "PE-myEnemy");
	}
	else if (!otherEnemies.empty())
	{
		std::sort(otherEnemies.begin(), otherEnemies.end(), compareEnemyDist);
		SetTarget(otherEnemies.front()->GetId(), otherEnemies.front()->GetTargetPosition(), "PE-otherEnemy");
	}
}
#pragma endregion ..\Codin\PeasantController.cpp
#pragma region ..\Codin\Simulate.cpp
// #include "Simulate.h"

// #include "Entity.h"
// #include "Game.h"
// #include "Rules.h"

#include <iostream>

#define LOG_SIMULATE_BEST_ATTACK 1

Vector Simulate::GetNearestBasePosition(const Entity& entity)
{
	return entity.GetPosition().x < Rules::mapSize.x / 2 ? Vector{ 0, 0 } : Rules::mapSize;;
}

void Simulate::Update(Entity& entity)
{
	entity.SetPosition(entity.GetTargetPosition());

	const Vector basePosition = GetNearestBasePosition(entity);
	if (Distance2(entity.GetPosition(), basePosition) <= Pow2(Rules::monsterBaseAttackRange))
	{
		entity.SetNearBase(true);
		entity.SetVelocity((basePosition - entity.GetPosition()).Lengthed(Rules::monsterMoveRange));
	}
}

Vector Simulate::PositionAfterFrames(const Entity& entity, int frames)
{
	if (frames == 0)
		return entity.GetPosition();

	Entity simulatedEnemy{ entity };
	for (int i = 0; i < frames; ++i)
		Update(simulatedEnemy);

	return simulatedEnemy.GetPosition();
}

int Simulate::EnemyFramesToAttackBase(const Entity& enemy)
{
	if (enemy.GetThreatFor() == ThreatFor::None)
		return std::numeric_limits<int>::max();

	const Vector basePosition = GetNearestBasePosition(enemy);

	constexpr int maxFrames2 = static_cast<int>(static_cast<float>(Rules::mapSize.Length2()) / static_cast<float>(Pow2(Rules::monsterMoveRange)));
	int framesToDealDamge = 0;
	Entity simulatedEnemy{ enemy };
	while (Distance2(simulatedEnemy.GetTargetPosition(), basePosition) > Pow2(Rules::monsterBaseDestroyRange)
		&& Pow2(framesToDealDamge) < maxFrames2)
	{
		Update(simulatedEnemy);
		++framesToDealDamge;
	}

	return framesToDealDamge;
}

int Simulate::HeroFramesToAttackEnemy(const Entity& hero, const Entity& enemy)
{
	constexpr int maxFrames2 = static_cast<int>(static_cast<float>(Rules::mapSize.Length2()) / static_cast<float>(Pow2(Rules::monsterMoveRange)));
	int framesToAttack = 0;
	Entity simulatedEnemy{ enemy };
	while (Distance2(hero.GetPosition(), enemy.GetPosition()) >= Pow2(Rules::heroMoveRange * (framesToAttack + 1) + Rules::heroAttackRange)
		&& Pow2(framesToAttack) < maxFrames2)
	{
		Update(simulatedEnemy);
		++framesToAttack;
	}

	return framesToAttack;
}

int Simulate::HeroFramesToCastSpell(const Entity& hero, const Entity& enemy, int spellRange)
{
	constexpr int maxFrames2 = static_cast<int>(static_cast<float>(Rules::mapSize.Length2()) / static_cast<float>(Pow2(Rules::monsterMoveRange)));
	int framesToSpell = 0;
	Entity simulatedEnemy{ enemy };
	while (Distance2(hero.GetPosition(), enemy.GetPosition()) >= Pow2(Rules::heroMoveRange * framesToSpell + spellRange)
		&& Pow2(framesToSpell) < maxFrames2)
	{
		Update(simulatedEnemy);
		++framesToSpell;
	}

	return framesToSpell;
}

int Simulate::FramesToKill(int healt)
{
	return healt / Rules::heroDamage;
}

Vector Simulate::GetBestAttackPosition(const Entity& hero, const Entity& danger, const Vector& preferedPosition, const Game& game)
{
	std::vector<const Entity*> nearestEnemies;
	nearestEnemies.reserve(16);

	// Find the nearest enemies that hero can attack together with the enemy.
	for (auto it : game.GetAllEntities())
	{
		const Entity* enemy = it.second.get();
		if (enemy->GetType() != EntityType::Monster)
			continue;
		if (Distance2(danger.GetPosition(), enemy->GetPosition()) > Pow2(Rules::heroAttackRange * 2))
			continue;

		nearestEnemies.push_back(enemy);
	}

	while (nearestEnemies.size() > 1)
	{
		Vector nearestPosition = GetPreferedAttackPosition(hero, danger, GetEnemiesCenter(nearestEnemies));

		bool canAttackAllNearest = true;
		for (auto it = nearestEnemies.begin(); it != nearestEnemies.end(); /* in loop */)
		{
			if (Distance2(nearestPosition, (*it)->GetPosition()) <= Pow2(Rules::heroAttackRange))
				++it;
			else
			{
				it = nearestEnemies.erase(it);
				canAttackAllNearest = false;
			}
		}

		if (canAttackAllNearest)
		{
#if LOG_SIMULATE_BEST_ATTACK
			std::cerr << " SBA:" << nearestEnemies.size() << std::endl;
#endif
			return nearestPosition;
		}
	}

	// There is no possibility of attacking more enemies than one.
	return GetPreferedAttackPosition(hero, danger, preferedPosition);
}

Vector Simulate::GetPreferedAttackPosition(const Entity& hero, const Entity& danger, const Vector& preferedPosition)
{
	Vector position = preferedPosition;

	constexpr int maxSteps = 10;
	int step = 0;
	while (step < maxSteps
		&& (Distance2(position, hero.GetPosition()) > Pow2(Rules::heroMoveRange)
			|| Distance2(position, danger.GetPosition()) > Pow2(Rules::heroAttackRange)))
	{
		position = hero.GetPosition() + (position - hero.GetPosition()).Limited(Rules::heroMoveRange);
		position = danger.GetPosition() + (position - danger.GetPosition()).Limited(Rules::heroAttackRange);
		++step;
	}

	return position;
}

Vector Simulate::GetEnemiesCenter(const std::vector<const Entity*>& enemies)
{
	Vector center{};
	for (const Entity* e : enemies)
		center += e->GetPosition();
	return center / std::max(static_cast<int>(enemies.size()), 1);
}
#pragma endregion ..\Codin\Simulate.cpp
#pragma region ..\Codin\StatsDescription.cpp
// #include "StatsDescription.h"

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

Vector Vector::Lengthed(int length) const
{
	if (!x && !y)
		return {};

	float flen = std::sqrt(static_cast<float>(Length2()));
	float fx = static_cast<float>(x) / flen * static_cast<float>(length);
	float fy = static_cast<float>(y) / flen * static_cast<float>(length);

	return { static_cast<int>(fx), static_cast<int>(fy) };
}

Vector Vector::Limited(int lenght) const
{
	return (Length2() <= Pow2(lenght)) ? *this : Lengthed(lenght);
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
			//std::cerr << entitesDesc.back() << std::endl; //< Uncomment to grab information about game input.
		}

		game.Tick(myStatsDesc, opponentStatsDesc, entitesDesc);
		game.MakeMove(std::cout);
	}
}
#pragma endregion Main.cpp
