#include "DeathState.h"
#include "DamageState.h"
#include "IdleState.h"
#include "ShootState.h"


State* DeathState::handleInput(Event t_event)
{

	return nullptr;
}

void DeathState::update(GameObject* obj)
{
	if (!(obj->holdAnimation(DEATH)))
	{
		obj->animation(DEATH);
	}
}

void DeathState::enter(GameObject* obj)
{
	obj->resetAnimation();
	obj->setDead();
}

void DeathState::exit(GameObject* obj)
{
}
