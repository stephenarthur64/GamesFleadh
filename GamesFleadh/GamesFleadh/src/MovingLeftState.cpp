#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"

State* MovingLeftState::handleInput(Event t_event)
{
    if (t_event == EVENT_NONE)
    {
        return new IdleState;
    }

    return nullptr;
}

void MovingLeftState::update(GameObject* obj)
{
}

void MovingLeftState::enter(GameObject* obj)
{
}

void MovingLeftState::exit(GameObject* obj)
{
}
