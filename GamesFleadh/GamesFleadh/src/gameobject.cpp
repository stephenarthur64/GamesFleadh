#include "../include/gameobject.h"
#include <raymath.h>

//// Initializes a GameObject with default values and assigns a name
//void InitGameObject(GameObject *obj, const char *name)
//{
//
//    // Initialize the previous and current states to IDLE
//    obj->previousState = STATE_IDLE;
//    obj->currentState = STATE_IDLE;
//
//    // Set the initial health to 100
//}
//
//// Frees memory associated with a GameObject, including state configurations
//void DeleteGameObject(GameObject *obj)
//{
//    // Check if state configurations exist for this GameObject
//    if (obj->stateConfigs)
//    {
//        // Free each state's nextStates array
//        for (int i = 0; i < STATE_COUNT; i++)
//        {
//            free(obj->stateConfigs[i].nextStates);
//        }
//        // Free the array of state configurations itself
//        free(obj->stateConfigs);
//    }
//    // Free the GameObject memory
//    free(obj);
//}

GameObject::GameObject() : m_position({ 7.0f, 0.0f, 0.0f }), m_colour(WHITE), m_pitch(0.0f), m_roll(0.0f), m_yaw(90.0f)
{
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
	m_body.transform = MatrixRotateXYZ({ DEG2RAD* m_pitch, DEG2RAD* m_yaw, DEG2RAD* m_roll });
	animIndex = index;
	ModelAnimation anim = modelAnimations[0];
	animCurrentFrame = (animCurrentFrame + 1) % anim.frameCount;
	bool fuckk = IsModelAnimationValid(m_body, anim);
	UpdateModelAnimation(m_body, anim, animCurrentFrame);
}

void GameObject::rotateYaw(int t_direction)
{
	int speed = 5;

	speed *= t_direction;

	if (m_yaw >= 60.0f && t_direction == -1)
	{
		m_yaw += speed;
	}

	if (m_yaw <= 130.0f && t_direction == 1)
	{
		m_yaw += speed;
	}
}
