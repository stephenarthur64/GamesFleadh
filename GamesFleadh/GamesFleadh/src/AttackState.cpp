#include "AttackState.h"
#include "IdleState.h"
#include "DeathState.h"
#include "DamageState.h"

State* AttackState::handleInput(Event t_event)
{
    if (t_event == EVENT_MOVE)
    {
        return new IdleState;
    }

    if (t_event == EVENT_DAMAGE)
    {
        return new DamageState;
    }

    if (t_event == EVENT_DIE)
    {
        return new DeathState;
    }

    return nullptr;
}

void AttackState::update(GameObject* obj)
{
    obj->animation(ATTACK);
}

void AttackState::enter(GameObject* obj)
{
    obj->resetAnimation();
}

void AttackState::exit(GameObject* obj)
{
}
