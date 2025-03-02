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
	const float getCollisionRadius() { return m_collisionRadius; }
	const float getBoundingBoxRadius() { return m_boundingBoxRadius; }
	int getHealth() { return m_health; }
	Vector2 getLowerLimit() { return lowerLimit; }
	Vector2 getUpperLimit() { return upperLimit; }
	void updateLimits(Vector2 t_low, Vector2 t_high);
	
	void reboundLimits(Vector3& t_cam);

	virtual void respawn() override;

	void addHealth(int t_amt) { m_health += t_amt; }
	int currentBullet() { return bulletCount; }
	void setHitBox();
	void updateHitBox(float t_x);
	void setAuto(bool t_auto) { m_auto = t_auto; }
	bool isAuto() { return m_auto; }

	Quaternion getCrosshairRotation() { return crosshairRotation; }
	
	void worldCollision(bool collide);
	void enemyCollision(bool collide);

	void updateZPos(float newXPos);
	virtual void rotate(int t_direction) override;
	virtual void init() override;
	virtual void render() override;
	void shootSound() override;
	void hitSound(int t_type);
	float getHealthBarHeight() { return m_healthbar.height; }
	Color getHealthBarColour() { return m_hpColour; }

	void update(Vector3& t_cam, Vector3 &t_crosshair);
	void updateHealthbar();

	void resetToOrigin();
	void faceCrosshair(Vector3 t_crosshairPos);
	void shootBullet(Vector3 t_target);
	void updateBullet();
	void despawnBullet(int bulletNum);

	void cameraMove(Vector3& t_cam);
	bool isAlive() { return m_alive; }

	void rebound(Vector3 t_impactPoint);
	// void rebound(Vector3 t_impactPoint);
	void reboundFurniture(FurnitureCollisionData t_data); // This possibly needs the t_cam var from above now
	
	void death(Vector3& t_cam, Vector3& t_target);

	void poisonPlayer(bool t_poison);
	bool isPoisoned() { return m_poisoned; }

	float boundingBoxRadius(BoundingBox box)
	{
		float dx = box.max.x - box.min.x;
		float dy = box.max.y - box.min.y;
		float dz = box.max.z - box.min.z;

		// Compute the diagonal length and divide by 2 to get the radius
		return 0.5f * sqrt(dx * dx + dy * dy + dz * dz);
	}



	void FORCEKILLDEBUG() { m_health = 0; }


private:
	//Weapon m_weapon;

	float m_speed;
	Vector3 m_currentVelocity = Vector3Zero();
	float hitboxOffsetMin = 1.0f;
	float hitboxOffsetMax = 1.0f;
	float m_collisionRadius = 1.0f;

	Bullet bullet[10];
	int bulletCount;
	const int MAX_BULLETS = 10;

	Sound shootingSFX;
	Sound environmentHitSFX;
	Sound enemyHitSFX;
	Sound buzzingSFX;
	Sound deathSFX;


	Quaternion crosshairRotation;
	Rectangle m_healthbar;
	Color m_hpColour;

	const float HEALTHBAR_MAX;

	Vector3 m_reboundDirection = Vector3Zero();
	float m_reboundCounter = 0.0f;
	Vector3 m_reboundCrosshair = Vector3Zero();
	const float m_reboundCountMax = 0.125f; // 33f;
	const float m_reboundForce = 5.0f;

	bool m_poisoned;
	int m_poisonTick;
	const int MAX_POISON_TICK;

	float m_boundingBoxRadius = 0.0f;

	bool m_auto = false;

	Vector2 lowerLimit = { -1.0f, -2.0f };
	Vector2 upperLimit = { 1.0f, 2.0f };
};

