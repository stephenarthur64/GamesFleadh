#include "NoInputState.h"
#include "IdleState.h"
#include "ShootState.h"
#include "DamageLState.h"
#include "DamageRState.h"

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
    if (t_event == EVENT_HIT_L)
    {
        return new DamageLState;
    }
    if (t_event == EVENT_HIT_R)
    {
        return new DamageRState;
    }
    return nullptr;
}

void NoInputState::update(GameObject* obj)
{
    obj->animation(IDLE_2);
}

void NoInputState::enter(GameObject* obj)
{
    obj->resetAnimation();
}

void NoInputState::exit(GameObject* obj)
{
}
