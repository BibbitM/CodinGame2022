#pragma once

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
