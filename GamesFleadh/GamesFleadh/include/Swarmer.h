#pragma once
#include "gameobject.h"
#include "IdleState.h"
#include "Globals.h"
#include <raymath.h>

#define NUM_FRAMES_PER_LINE     5
#define NUM_LINES               5

class Swarmer :
    public GameObject
{
public:
	Swarmer();

	virtual void rotate(int t_direction);
	virtual void init();
	virtual void render();
	void renderBoom(Camera t_camera);

	void boom();

	void setLimits(float t_upperLimit, float t_lowerLimit);
	void setHitbox();

	void collision(bool t_collide);
	void kill();

	void update();
	void playerSpotted(bool t_spotted);
	void hover();

	void checkDistanceFromPlayer(Vector3 t_playerPos);
	void chasePlayer();

private:
	int spottedTick = 0;

	float m_upperLimit;
	float m_lowerLimit;

	float m_speed;
	Direction m_direction;
	bool m_spotted;

	Vector3 m_target;
	Vector3 m_velocity;

	const float MAX_DISTANCE;

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

