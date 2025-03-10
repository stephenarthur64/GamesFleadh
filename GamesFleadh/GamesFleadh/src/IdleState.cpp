#include "IdleState.h"
#include "ShootState.h"
#include "PlayerSpottedState.h"
#include "DamageLState.h"
#include "DamageRState.h"
#include "DamageState.h"
#include "NoInputState.h"
#include "EatenState.h"
#include "AttackState.h"
#include "CollisionLState.h"
#include "CollisionRState.h"

State* IdleState::handleInput(Event t_event)
{
	if (t_event == EVENT_NONE)
	{
		return new NoInputState;
	}

	if (t_event == EVENT_SHOOT)
	{
		return new ShootState;
	}

	if (t_event == EVENT_HIT_L)
	{
		return new DamageLState;
	}
	if (t_event == EVENT_HIT_R)
	{
		return new DamageRState;
	}

	if (t_event == EVENT_DAMAGE)
	{
		return new DamageState;
	}

	if (t_event == EVENT_EAT)
	{
		return new EatenState;
	}

	if (t_event == Event::EVENT_SPOTTED)
	{
		return new PlayerSpottedState;
	}

	if (t_event == EVENT_ATTACK)
	{
		return new AttackState;
	}

	if (t_event == EVENT_COLLIDE_L)
	{
		return new CollisionLState;
	}
	if (t_event == EVENT_COLLIDE_R)
	{
		return new CollisionRState;
	}

	return nullptr;
}

void IdleState::update(GameObject* obj)
{
	obj->animation(DEFAULT);	
}

void IdleState::enter(GameObject* obj)
{
}

void IdleState::exit(GameObject* obj)
{
}
