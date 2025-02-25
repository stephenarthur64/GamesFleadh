#pragma once
#include <iostream>
#include <raylib.h>
#include "Globals.h"
#include "Bullet.h"
#include "raymath.h"
#include "gameobject.h"
#include "State.h"
#include "NoInputState.h"
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
	void addHealth(int t_amt) { m_health += t_amt; }
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
	void shootSound() override;
	void hitSound(int t_type);
	Rectangle getHealthBar() { return m_healthbar; }
	Color getHealthBarColour() { return m_hpColour; }

	void update();
	void updateHealthbar();

	void resetToOrigin();
	void faceCrosshair(Vector3 t_crosshairPos);

	void shootBullet(Vector3 t_target);
	void updateBullet();
	void despawnBullet(int bulletNum);

	void rebound(Vector3 t_impactPoint);
	
	void poisonPlayer(bool t_poison);
	bool isPoisoned() { return m_poisoned; }

private:
	//Weapon m_weapon;

	float m_speed;
	float hitboxOffsetMin = 1.0f;
	float hitboxOffsetMax = 1.0f;

	Bullet bullet[10];
	int bulletCount;
	const int MAX_BULLETS = 10;

	Sound shootingSFX;
	Sound environmentHitSFX;
	Sound enemyHitSFX;

	Quaternion crosshairRotation;
	Rectangle m_healthbar;
	Color m_hpColour;

	const float HEALTHBAR_MAX;

	Vector3 m_reboundDirection = Vector3Zero();
	float m_reboundCounter = 0.0f;
	const float m_reboundCountMax = 0.125f; // 33f;
	const float m_reboundForce = 20.0f;

	bool m_poisoned;
	int m_poisonTick;
	const int MAX_POISON_TICK;
};

