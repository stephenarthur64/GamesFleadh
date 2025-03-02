#pragma once
#include <raylib.h>
#include "gameobject.h"
#include "IdleState.h"
#include "Bullet.h"
#include "Globals.h"
#include <raymath.h>

#define NUM_FRAMES_PER_LINE     5
#define NUM_LINES               5

class Feeder : public GameObject
{
public:
	Feeder();

	void setHitBox();
	void updateHitBox();
	void spawn(Vector3 t_position);
	void collision(bool t_collision);
	virtual void init() override;
	virtual void render() override;

	void renderBoom(Camera& t_camera);

	Model* getBulletModel(int count) { return m_mudBomb.getModel(); }
	Vector3 getBulletPositon(int count) { return m_mudBomb.getPosition(); }
	BoundingBox getBulletHitBox(int count) { return m_mudBomb.getHitbox(); }
	
	bool checkBulletCollisions(BoundingBox t_player);

	void shootBullet(Vector3 t_target);
	void despawnBullet();
	void disableShooting();

	void boom();
	bool isActive() { return boomActive; }

	void makeActive() { m_active = true; }

	void kill();

	bool isAlive();

	void update(Vector3 t_target);

	void checkDistanceFromPlayer(Vector3 t_playerPos);

	virtual void rotate(int t_direction) override;

private:
	Bullet m_mudBomb;

	float hitboxOffsetMin = 9.0f;
	float hitboxOffsetMax = 11.0f;

	int bulletTick = -1;
	int damageTick = -1;

	const int BULLET_TICK_MAX;
	const int DAMAGE_TICK_MAX;

	Sound sfxDeath;
	Sound sfxHit;
	Sound sfxFeeding;
	Sound sfxMissileHit;
	Sound sfxMissileLaunch;

	// Load explosion texture
	Texture2D explosion;

	// Init variables for animation
	float frameWidth;   // Sprite one frame rectangle width
	float frameHeight;           // Sprite one frame rectangle height
	int currentFrame = 0;
	int currentLine = 0;

	Rectangle frameRec;
	Vector2 position = { 0.0f, 0.0f };

	bool boomActive = false;
	int framesCounter = 0;

	Vector3 m_target;

	bool m_active;
	bool m_spotted;
	const float MAX_DISTANCE;
};

