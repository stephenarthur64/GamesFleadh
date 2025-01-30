#pragma once
#include <raylib.h>

class Enemy
{
public:
	Enemy();
	Model* getModel() { return &m_body; }
	Vector3 getPositon() { return m_position; }
	BoundingBox getHitbox() { return m_hitbox; }
	void setHitBox();
	void updateHitBox();

private:
	Model m_body;
	Vector3 m_position;
	BoundingBox m_hitbox;
};

