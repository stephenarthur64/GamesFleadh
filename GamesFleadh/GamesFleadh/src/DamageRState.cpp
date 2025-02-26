#include "DamageRState.h"
#include "IdleState.h"
#include "ShootState.h"
#include "NoInputState.h"

State* DamageRState::handleInput(Event t_event)
{
    if (m_tick >= 30)
    {
        if (t_event == EVENT_MOVE)
        {
            return new IdleState;
        }
        if (t_event == EVENT_SHOOT)
        {
            return new ShootState;
        }
        if (t_event == EVENT_NONE)
        {
            return new NoInputState;
        }
    }
    return nullptr;
}

void DamageRState::update(GameObject* obj)
{
    obj->animation(HIT_R);
    m_tick++;
}

void DamageRState::enter(GameObject* obj)
{
    obj->resetAnimation();
}

void DamageRState::exit(GameObject* obj)
{
}
