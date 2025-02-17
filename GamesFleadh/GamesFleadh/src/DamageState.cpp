#include "DamageState.h"
#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"

State* DamageState::handleInput(Event t_event)
{
    if (t_event == EVENT_NONE)
    {
        return new IdleState;
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
