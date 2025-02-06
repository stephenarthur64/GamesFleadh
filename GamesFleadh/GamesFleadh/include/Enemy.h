#pragma once
#include <raylib.h>
#include "gameobject.h"

class Enemy : public GameObject
{
public:
	Enemy();

	void setHitBox();
	void updateHitBox();

	void collision(bool t_collision);

private:


	float hitboxOffsetMin = 5.0f;
	float hitboxOffsetMax = 1.0f;
};

