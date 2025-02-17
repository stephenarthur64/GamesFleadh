#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"
#include "DamageState.h"
#include "ShootState.h"

State* MovingDownState::handleInput(Event t_event)
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

void MovingDownState::update(GameObject* obj)
{
    obj->animation(0);
}

void MovingDownState::enter(GameObject* obj)
{
}

void MovingDownState::exit(GameObject* obj)
{
}
