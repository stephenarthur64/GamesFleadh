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

private:
	Enemy enemy;
};

