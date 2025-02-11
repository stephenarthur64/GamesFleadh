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
    Vector3 getPositon() { return m_position; }
    BoundingBox getHitbox() { return m_hitbox; }
    Color getColour() { return m_colour; }
	virtual void handleInput(Event t_event);
	virtual void rotate(int t_direction) = 0;
	void animation(int index);

protected:
	Model m_body;
	Vector3 m_position;
	BoundingBox m_hitbox;
	Color m_colour;
	State* currentState;

	int animsCount;
	unsigned int animIndex;
	unsigned int animCurrentFrame;
	ModelAnimation* modelAnimations;
};






