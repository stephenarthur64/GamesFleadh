#pragma once
#include <raylib.h>
#include "gameobject.h"
#include "IdleState.h"
#include "Bullet.h"
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

	void shootBullet();
	void despawnBullet();
	void disableShooting();

	void boom();
	bool isActive() { return active; }

	void kill();

	void update();

	virtual void rotate(int t_direction) override;

private:
	Bullet m_mudBomb;

	float hitboxOffsetMin = 9.0f;
	float hitboxOffsetMax = 11.0f;

	int bulletTick = -1;
	int damageTick = -1;

	Sound fxBoom;

	// Load explosion texture
	Texture2D explosion;

	// Init variables for animation
	float frameWidth;   // Sprite one frame rectangle width
	float frameHeight;           // Sprite one frame rectangle height
	int currentFrame = 0;
	int currentLine = 0;

	Rectangle frameRec;
	Vector2 position = { 0.0f, 0.0f };

	bool active = false;
	int framesCounter = 0;

};

