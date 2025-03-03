#include "StateChase.h"
#include "IdleState.h"
#include "ShootState.h"
#include "DeathState.h"

State* StateChase::handleInput(Event t_event)
{
    if (t_event == EVENT_MOVE)
    {
        return new IdleState;
    }
    if (t_event == EVENT_SHOOT)
    {
        return new ShootState;
    }

    if (t_event == EVENT_DIE)
    {
        return new DeathState;
    }

    return nullptr;
}

void StateChase::update(GameObject* obj)
{
    obj->animation(CHASE);
}

void StateChase::enter(GameObject* obj)
{
}

void StateChase::exit(GameObject* obj)
{
}
