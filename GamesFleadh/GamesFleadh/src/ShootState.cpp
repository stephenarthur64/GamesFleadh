#include "ShootState.h"
#include "DeathState.h"
#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"
#include "DamageLState.h"
#include "DamageRState.h"

State* ShootState::handleInput(Event t_event)
{
	if (t_event == Event::EVENT_MOVE && tick >= 30)
	{
		return new IdleState;
	}

	if (t_event == Event::EVENT_NONE && tick >= 30)
	{
		return new NoInputState;
	}

	if (t_event == EVENT_HIT_L)
	{
		return new DamageLState;
	}
	if (t_event == EVENT_HIT_R)
	{
		return new DamageRState;
	}

	return nullptr;
}

void ShootState::update(GameObject* obj)
{
	tick++;
	//obj->holdAnimation(0, 3);
	obj->animation(1);
}

void ShootState::enter(GameObject* obj)
{
	obj->resetAnimation();
	obj->shootSound();
}

void ShootState::exit(GameObject* obj)
{
	tick = 0;
}
