#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"
#include "ShootState.h"
#include "PlayerSpottedState.h"
#include "DamageLState.h"
#include "DamageRState.h"

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

	if (t_event == Event::EVENT_SPOTTED)
	{
		return new PlayerSpottedState;
	}

	return nullptr;
}

void IdleState::update(GameObject* obj)
{
	obj->animation(0);	
}

void IdleState::enter(GameObject* obj)
{
}

void IdleState::exit(GameObject* obj)
{
}
