#include "DeathState.h"
#include "DamageState.h"
#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"
#include "ShootState.h"


State* DeathState::handleInput(Event t_event)
{
	if (t_event == Event::EVENT_NONE)
	{
		return new IdleState;
	}

	return nullptr;
}

void DeathState::update(GameObject* obj)
{
	obj->animation(2);
}

void DeathState::enter(GameObject* obj)
{
	obj->resetAnimation();
}

void DeathState::exit(GameObject* obj)
{
}
