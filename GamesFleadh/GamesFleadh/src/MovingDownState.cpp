#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"
#include "DamageState.h"

State* MovingDownState::handleInput(Event t_event)
{
    if (t_event == EVENT_NONE)
    {
        return new IdleState;
    }

    if (t_event == Event::EVENT_DAMAGE)
    {
        return new DamageState;
    }

    return nullptr;
}

void MovingDownState::update(GameObject* obj)
{
    obj->animation(1);
}

void MovingDownState::enter(GameObject* obj)
{
}

void MovingDownState::exit(GameObject* obj)
{
}
