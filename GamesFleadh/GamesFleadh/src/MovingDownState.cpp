#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"

State* MovingDownState::handleInput(Event t_event)
{
    if (t_event == EVENT_NONE)
    {
        return new IdleState;
    }

    return nullptr;
}

void MovingDownState::update(GameObject* obj)
{
}

void MovingDownState::enter(GameObject* obj)
{
}

void MovingDownState::exit(GameObject* obj)
{
}
