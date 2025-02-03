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
	Model* getBulletModel(int count) { return bullet[count].getModel(); }
	Vector3 getPositon() { return m_position; }
	Vector3 getBulletPositon(int count) { return bullet[count].getPositon(); }
	BoundingBox getHitbox() { return m_hitbox; }
	BoundingBox getBulletHitBox(int count) { return bullet[count].getHitbox(); }
	const int getBulletMax() { return MAX_BULLETS; }
	int currentBullet() { return bulletCount; }
	void setHitBox();
	void updateHitBox(float t_x);
	Color getColor() { return m_colour; }
	void collision(bool collide);

	void shootBullet();
	void updateBullet();
	void despawnBullet(int bulletNum);


private:
	Vector3 m_position;
	float m_speed;
	Model m_body;
	BoundingBox m_hitbox;
	Color m_colour;

	float hitboxOffsetMin = 6.0f;
	float hitboxOffsetMax = 7.0f;

	Bullet bullet[10];
	int bulletCount;
	const int MAX_BULLETS = 10;
};

