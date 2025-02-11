#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"

State* MovingRightState::handleInput(Event t_event)
{
    if (t_event == EVENT_NONE)
    {
        return new IdleState;
    }

    return nullptr;
}

void MovingRightState::update(GameObject* obj)
{
    obj->animation(5);
}

void MovingRightState::enter(GameObject* obj)
{
}

void MovingRightState::exit(GameObject* obj)
{
}
