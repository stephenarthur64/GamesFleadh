#pragma once
#include "gameobject.h"
#include "IdleState.h"
#include "Globals.h"

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

private:
	int spottedTick = 0;

	float m_upperLimit;
	float m_lowerLimit;

	float m_speed;
	Direction m_direction;
};

