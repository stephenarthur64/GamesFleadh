#pragma once
#include <raylib.h>
#include "gameobject.h"
#include "IdleState.h"

class Enemy : public GameObject
{
public:
	Enemy();

	void setHitBox();
	void updateHitBox();
	void spawn(Vector3 t_position);
	void collision(bool t_collision);
	virtual void init() override;
	virtual void render() override;

	void kill();

	void update();

	virtual void rotate(int t_direction) override;

private:


	float hitboxOffsetMin = 9.0f;
	float hitboxOffsetMax = 11.0f;
};

