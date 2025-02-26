#include "DamageLState.h"
#include "IdleState.h"
#include "ShootState.h"
#include "NoInputState.h"

State* DamageLState::handleInput(Event t_event)
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

void DamageLState::update(GameObject* obj)
{
    obj->animation(HIT_L);
    m_tick++;
}

void DamageLState::enter(GameObject* obj)
{
    obj->resetAnimation();
}

void DamageLState::exit(GameObject* obj)
{
}
