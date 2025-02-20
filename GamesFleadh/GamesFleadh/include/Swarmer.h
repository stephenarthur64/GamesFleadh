#pragma once
#include "gameobject.h"
#include "IdleState.h"
#include "Globals.h"
#include <raymath.h>

class Swarmer :
    public GameObject
{
public:
	Swarmer();

	virtual void rotate(int t_direction);
	virtual void init();
	virtual void render();

	void setLimits(float t_upperLimit, float t_lowerLimit);

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
};

