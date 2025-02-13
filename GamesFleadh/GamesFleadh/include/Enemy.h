#pragma once
#include <raylib.h>
#include "gameobject.h"

class Enemy : public GameObject
{
public:
	Enemy();

	void setHitBox();
	void updateHitBox();
	void spawn();
	void collision(bool t_collision);

	virtual void rotate(int t_direction) override;

private:


	float hitboxOffsetMin = 9.0f;
	float hitboxOffsetMax = 11.0f;
};

