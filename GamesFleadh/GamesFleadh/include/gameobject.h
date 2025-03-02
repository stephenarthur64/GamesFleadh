#pragma once

#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "events.h"
#include <State.h>



class GameObject {
public:
	GameObject();

    Model* getModel() { return &m_body; }
    Vector3 getPosition() { return m_position; }
    BoundingBox getHitbox() { return m_hitbox; }
	BoundingBox getCurrentBounds();
    Color getColour() { return m_colour; }
	virtual void handleInput(Event t_event);
	virtual void rotate(int t_direction) = 0;
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void respawn() {};
	virtual void shootSound() {};
	void animation(int index);
	void rotateYaw(int t_direction);

	void resetAnimation();
	bool holdAnimation(int t_index);
	void setDead() { m_alive = false; }

	bool m_inPlay = false;

protected:
	Model m_body;
	Vector3 m_position;
	BoundingBox m_hitbox;
	Color m_colour;
	State* currentState;

	bool m_alive;

	float m_roll;
	float m_pitch;
	float m_yaw;

	int m_health;

	int animsCount;
	unsigned int animIndex;
	unsigned int animCurrentFrame;
	ModelAnimation* modelAnimations;
};






