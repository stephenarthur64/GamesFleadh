#include "EatenState.h"
#include "IdleState.h"

State* EatenState::handleInput(Event t_event)
{
    if (t_event == EVENT_MOVE)
    {
        return new IdleState;
    }
    return nullptr;
}

void EatenState::update(GameObject* obj)
{
    obj->animation(1);
}

void EatenState::enter(GameObject* obj)
{
    obj->resetAnimation();
}

void EatenState::exit(GameObject* obj)
{
}
