#include "PlayerSpottedState.h"
#include "IdleState.h"
#include "DamageState.h"
#include "ShootState.h"
#include "StateChase.h"
#include "DeathState.h"

State* PlayerSpottedState::handleInput(Event t_event)
{
    if (t_event == Event::EVENT_MOVE)
    {
        return new IdleState;
    }

    if (t_event == EVENT_DIE)
    {
        return new DeathState;
    }

    if (t_event == Event::EVENT_CHASE)
    {
        return new StateChase;
    }
    return nullptr;
}

void PlayerSpottedState::update(GameObject* obj)
{
    obj->animation(ALERT);
}

void PlayerSpottedState::enter(GameObject* obj)
{
    obj->resetAnimation();
}

void PlayerSpottedState::exit(GameObject* obj)
{
}
