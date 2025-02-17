#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"
#include "DamageState.h"
#include "ShootState.h"

State* MovingUpState::handleInput(Event t_event)
{
    if (t_event == EVENT_NONE)
    {
        return new IdleState;
    }

    if (t_event == EVENT_SHOOT)
    {
        return new ShootState;
    }

    if (t_event == Event::EVENT_DAMAGE)
    {
        return new DamageState;
    }

    return nullptr;
}

void MovingUpState::update(GameObject* obj)
{
    obj->animation(0);
}

void MovingUpState::enter(GameObject* obj)
{
}

void MovingUpState::exit(GameObject* obj)
{
}
