#include "StateChase.h"
#include "IdleState.h"

State* StateChase::handleInput(Event t_event)
{
    if (t_event == EVENT_MOVE)
    {
        return new IdleState;
    }
    return nullptr;
}

void StateChase::update(GameObject* obj)
{
    obj->animation(2);
}

void StateChase::enter(GameObject* obj)
{
}

void StateChase::exit(GameObject* obj)
{
}
