#include "../include/gameobject.h"
#include <raymath.h>

//GameObject::GameObject() : m_position({ 7.0f, 0.0f, 0.0f }), m_colour(WHITE), m_pitch(0.0f), m_roll(0.0f), m_yaw(90.0f)
//{
//}

GameObject::GameObject() : m_position({ 0.0f, 3.0f, 0.0f }), m_colour(WHITE), m_pitch(0.0f), m_roll(0.0f), m_yaw(90.0f)
{
}

BoundingBox GameObject::getCurrentBounds()
{
	BoundingBox box;

	box.min.x = getHitbox().min.x + m_position.x;
	box.max.x = getHitbox().min.x + m_position.x;

	box.min.y = getHitbox().min.y + m_position.y;
	box.max.y = getHitbox().min.y + m_position.y;

	box.min.z = getHitbox().min.z + m_position.z;
	box.max.z = getHitbox().min.z + m_position.z;

	return BoundingBox();
}

void GameObject::handleInput(Event t_event)
{
	State* tempState = nullptr;

	if (currentState)
	{
		tempState = currentState->handleInput(t_event);
	}

	if (tempState != NULL)
	{
		currentState->exit(this);
		delete currentState;
		currentState = tempState;
		currentState->enter(this);
	}
}

void GameObject::animation(int index)
{
	// m_body.transform = MatrixRotateXYZ({ DEG2RAD * m_pitch, DEG2RAD * m_yaw, DEG2RAD * m_roll });
	animIndex = index;
	ModelAnimation anim = modelAnimations[animIndex]; // if index must be changed, go here (not over 1, breaks Buzzz)
	animCurrentFrame = (animCurrentFrame + 1) % anim.frameCount;
 	UpdateModelAnimation(m_body, anim, animCurrentFrame);
}

void GameObject::rotateYaw(int t_direction)
{
	int speed = 3;

	speed *= t_direction;

	if (m_yaw >= 85.0f && t_direction == -1)
	{
		m_yaw += speed;
	}

	if (m_yaw <= 95.0f && t_direction == 1)
	{
		m_yaw += speed;
	}
}

void GameObject::resetAnimation()
{
	animCurrentFrame = 1;
}

bool GameObject::holdAnimation(int t_index)
{
	int frameMax = modelAnimations[t_index].frameCount;

	if (animCurrentFrame >= frameMax - 1)
	{
		animCurrentFrame = frameMax;
		return true;
	}
	return false;
}
