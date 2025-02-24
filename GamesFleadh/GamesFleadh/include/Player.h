#pragma once
#include <iostream>
#include <raylib.h>
#include "Globals.h"
#include "Bullet.h"
#include "raymath.h"
#include "gameobject.h"
#include "State.h"
#include "IdleState.h"
#include "Weapon.h"

class Player : public GameObject
{
public:
	Player();
	void move(Vector3 t_velocity);
	Model* getBulletModel(int count) { return bullet[count].getModel(); }
	Vector3 getBulletPositon(int count) { return bullet[count].getPosition(); }
	BoundingBox getBulletHitBox(int count) { return bullet[count].getHitbox(); }
	const int getBulletMax() { return MAX_BULLETS; }
	int currentBullet() { return bulletCount; }
	void setHitBox();
	void updateHitBox(float t_x);

	Quaternion getCrosshairRotation() { return crosshairRotation; }
	
	void worldCollision(bool collide);
	void enemyCollision(bool collide);

	void updateZPos(float newXPos);
	virtual void rotate(int t_direction) override;
	virtual void init() override;
	virtual void render() override;
	Rectangle getHealthBar() { return m_healthbar; }

	void update();
	void updateHealthbar();

	void resetToOrigin();
	void faceCrosshair(Vector3 t_crosshairPos);

	void shootBullet(Vector3 t_target);
	void updateBullet();
	void despawnBullet(int bulletNum);

private:
	//Weapon m_weapon;

	float m_speed;
	float hitboxOffsetMin = 1.0f;
	float hitboxOffsetMax = 3.0f;

	Bullet bullet[10];
	int bulletCount;
	const int MAX_BULLETS = 10;

	Quaternion crosshairRotation;
	Rectangle m_healthbar;

	const float HEALTHBAR_MAX;
};

