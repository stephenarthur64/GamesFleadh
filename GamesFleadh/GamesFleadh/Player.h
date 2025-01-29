#pragma once
#include <raylib.h>
#include "Globals.h"
#include "Bullet.h"

class Player
{
public:
	Player();
	void move(int t_directon);
	Model* getModel() { return &m_body; }
	Model* getBulletModel() { return bullet.getModel(); }
	Vector3 getPositon() { return m_position; }
	Vector3 getBulletPositon() { return bullet.getPositon(); }
	BoundingBox getHitbox() { return m_hitbox; }
	BoundingBox getBulletHitBox() { return bullet.getHitbox(); }

private:
	Vector3 m_position;
	float m_speed;
	Model m_body;
	BoundingBox m_hitbox;

	Bullet bullet;
};

