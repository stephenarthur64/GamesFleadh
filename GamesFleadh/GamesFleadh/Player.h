#pragma once
#include <raylib.h>
#include "Globals.h"

class Player
{
public:
	Player();
	void move(int t_directon);

private:
	Vector3 m_position;
	float m_speed;
};

