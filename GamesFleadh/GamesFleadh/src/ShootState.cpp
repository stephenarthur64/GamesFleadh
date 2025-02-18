#include "ShootState.h"
#include "DeathState.h"
#include "DamageState.h"
#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"

State* ShootState::handleInput(Event t_event)
{
	if (t_event == Event::EVENT_NONE && tick >= 60)
	{
		return new IdleState;
	}

	if (t_event == Event::EVENT_DAMAGE)
	{
		return new DamageState;
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
}

void ShootState::exit(GameObject* obj)
{
	tick = 0;
}
