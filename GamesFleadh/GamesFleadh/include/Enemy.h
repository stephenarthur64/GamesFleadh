#pragma once
#include <raylib.h>

class Enemy
{
public:
	Enemy();
	Model* getModel() { return &m_body; }
	Vector3 getPositon() { return m_position; }
	BoundingBox getHitbox() { return m_hitbox; }
	Color getColour() { return m_colour; }
	void setHitBox();
	void updateHitBox();

	void collision(bool t_collision);

private:
	Model m_body;
	Vector3 m_position;
	BoundingBox m_hitbox;
	Color m_colour;

	float hitboxOffsetMin = 5.0f;
	float hitboxOffsetMax = 1.0f;
};

