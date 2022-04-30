#pragma once

class Entity;
class Game;
struct Vector;

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

	static Vector GetBestAttackPosition(const Entity& hero, const Entity& enemy, const Vector& preferedPosition, const Game& game);
};
