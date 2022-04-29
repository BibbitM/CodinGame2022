#pragma once

class Entity;
struct Vector;

class Simulate
{
public:
	static Vector GetNearestBasePosition(const Entity& entity);
	static void Update(Entity& entity);
	static int EnemyFramesToDealDamage(const Entity& entity);
};
