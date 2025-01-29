#pragma once
#include <raylib.h>

class Bullet
{
public:
	Bullet();
	Model* getModel() { return &m_body; }
	Vector3 getPositon() { return m_position; }

private:
	Model m_body;
	Vector3 m_position;
};

