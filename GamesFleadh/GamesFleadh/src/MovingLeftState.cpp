#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"
#include "DamageState.h"
#include "ShootState.h"

State* MovingLeftState::handleInput(Event t_event)
{
    if (t_event == EVENT_MOVE)
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

void MovingLeftState::update(GameObject* obj)
{
    obj->animation(0);
    //obj->rotateYaw(-1);
}

void MovingLeftState::enter(GameObject* obj)
{
}

void MovingLeftState::exit(GameObject* obj)
{
}
