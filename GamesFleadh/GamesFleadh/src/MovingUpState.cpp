#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"

State* MovingUpState::handleInput(Event t_event)
{
    if (t_event == EVENT_NONE)
    {
        return new IdleState;
    }

    return nullptr;
}

void MovingUpState::update(GameObject* obj)
{
}

void MovingUpState::enter(GameObject* obj)
{
}

void MovingUpState::exit(GameObject* obj)
{
}
