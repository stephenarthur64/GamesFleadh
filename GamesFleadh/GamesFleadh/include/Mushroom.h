#pragma once
#include "gameobject.h"
#include "Enemy.h"

class Mushroom : public GameObject
{
public:
	Mushroom();
	virtual void rotate(int t_direction) override;
	virtual void init() override;
	virtual void render() override;

	BoundingBox getEnemyHitbox() { return enemy.getHitbox(); }
	void setCollisions(bool t_collide) { enemy.collision(t_collide); }
	GameObject* getEnemy() { return &enemy; }

	void playerDetected(bool t_spotted);

	void update();

	void spawn(Vector3 t_pos);
	void spawnEnemy();

private:
	Enemy enemy;
};

