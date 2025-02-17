#pragma once
#include <raylib.h>
#include "gameobject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	void setHitBox();

	void spawn(Vector3 t_pos, float speed);
	void despawn();
	void move();
	virtual void init() override;
	virtual void render() override;
	virtual void rotate(int t_direction) override;

private:
	float m_speed;
	bool m_active;
};

