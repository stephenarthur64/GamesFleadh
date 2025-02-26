#include "DamageState.h"
#include "IdleState.h"
#include "DeathState.h"

State* DamageState::handleInput(Event t_event)
{
    if (t_event == EVENT_MOVE)
    {
        return new IdleState;
    }
    if (t_event == EVENT_DIE)
    {
        return new DeathState;
    }

    return nullptr;
}

void DamageState::update(GameObject* obj)
{
    obj->animation(1);
}

void DamageState::enter(GameObject* obj)
{
    obj->resetAnimation();
}

void DamageState::exit(GameObject* obj)
{
}
