#include "NoInputState.h"
#include "IdleState.h"
#include "ShootState.h"

State* NoInputState::handleInput(Event t_event)
{
    if (t_event == EVENT_MOVE)
    {
        return new IdleState;
    }
    if (t_event == EVENT_SHOOT)
    {
        return new ShootState;
    }
    return nullptr;
}

void NoInputState::update(GameObject* obj)
{
    obj->animation(7);
}

void NoInputState::enter(GameObject* obj)
{
    obj->resetAnimation();
}

void NoInputState::exit(GameObject* obj)
{
}
