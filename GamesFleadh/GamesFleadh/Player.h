#pragma once
#include <raylib.h>
#include "Globals.h"

class Player
{
public:
	Player();
	void move(int t_directon);
	Model* getModel() { return &m_body; }
	Vector3 getPositon() { return m_position; }

private:
	Vector3 m_position;
	float m_speed;
	Model m_body;
};

