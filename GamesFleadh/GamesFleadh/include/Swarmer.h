#pragma once
#include "gameobject.h"
#include "IdleState.h"
#include "Globals.h"
#include "reasings.h"
#include <raymath.h>
#include <player.h>

#define NUM_FRAMES_PER_LINE     5
#define NUM_LINES               5

class Swarmer :
    public GameObject
{
public:
	// Swarmer(Player* t_playerRef);
	Swarmer();
	// ~Swarmer();

	virtual void rotate(int t_direction);
	virtual void init();
	virtual void render();
	void renderBoom(Camera t_camera);

	void spawn(Vector3 t_position, float limitMax, float limitMin);

	void boom();
	void colour(Color ye) { m_colour = RED; }
	void setLimits(float t_upperLimit, float t_lowerLimit);
	void setHitbox();

	void collision(bool t_collide);
	void kill();

	bool isActive() { return active; }

	void update();
	void playerSpotted(bool t_spotted);
	void hover();

	void checkDistanceFromPlayer(Vector3 t_playerPos);
	void chasePlayer();

private:
	int spottedTick = 0;
	int hoverTick = 0;

	float m_upperLimit;
	float m_lowerLimit;

	float m_speed;
	Direction m_direction;
	bool m_spotted;

	Vector3 m_target;
	Vector3 m_velocity;

	const float MAX_DISTANCE;
	//we bring the boom-jamie cullen
	Sound sfxDie;
	Sound sfxAlert;
	Sound sfxAttack;
	Sound sfxScan;

	// Load explosion texture
	Texture2D explosion;
	Color boomColor;

	// Init variables for animation
	float frameWidth;   // Sprite one frame rectangle width
	float frameHeight;           // Sprite one frame rectangle height
	int currentFrame = 0;
	int currentLine = 0;

	Rectangle frameRec;
	Vector2 position = { 0.0f, 0.0f };

	bool active = false;
	int framesCounter = 0;
	float boomScale = 2.0f;

	Player* m_playerReference;
};

