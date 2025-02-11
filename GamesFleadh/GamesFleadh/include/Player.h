#pragma once
#include <raylib.h>
#include "Globals.h"
#include "Bullet.h"
#include "raymath.h"
#include "gameobject.h"
#include "State.h"
#include "IdleState.h"

class Player : public GameObject
{
public:
	Player();
	void move(Vector3 t_velocity);
	Model* getBulletModel(int count) { return bullet[count].getModel(); }
	Vector3 getBulletPositon(int count) { return bullet[count].getPositon(); }
	BoundingBox getBulletHitBox(int count) { return bullet[count].getHitbox(); }
	const int getBulletMax() { return MAX_BULLETS; }
	int currentBullet() { return bulletCount; }
	void setHitBox();
	void updateHitBox(float t_x);
	void collision(bool collide);
	void updateZPos(float newXPos);
	virtual void rotate(int t_direction) override;

	void update();

	void resetToOrigin();

	void shootBullet();
	void updateBullet();
	void despawnBullet(int bulletNum);

private:
	float m_speed;
	float m_roll;
	float m_pitch;

	float hitboxOffsetMin = 1.0f;
	float hitboxOffsetMax = 3.0f;

	Bullet bullet[10];
	int bulletCount;
	const int MAX_BULLETS = 10;
};

