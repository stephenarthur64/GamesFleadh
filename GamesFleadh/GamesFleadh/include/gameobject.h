#pragma once

#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "events.h"

#include <State.h>



class GameObject {



public:
    GameObject() : m_position({ 7.0f, 0.0f, 0.0f }), m_colour(WHITE) {}

    Model* getModel() { return &m_body; }
    Vector3 getPositon() { return m_position; }
    BoundingBox getHitbox() { return m_hitbox; }
    Color getColour() { return m_colour; }


	void update()
	{
		currentState->handleInput(this);
	}

	void setState(State* newState)
	{
		if (currentState)
		{
			currentState->exit(this);
		}
		currentState = newState;
		currentState->enter(this);
	}

protected:
	Model m_body;
	Vector3 m_position;
	BoundingBox m_hitbox;
	Color m_colour;
	State* currentState;
};






