#include "CollisionRState.h"
#include "IdleState.h"
#include "ShootState.h"
#include "DamageLState.h"
#include "NoInputState.h"

State* CollisionRState::handleInput(Event t_event)
{
    

    if (m_tick >= 62)
    {
        if (t_event == EVENT_SHOOT)
        {
            return new ShootState;
        }

        if (t_event == EVENT_MOVE)
        {
            return new IdleState;
        }
        
        if (t_event == EVENT_NONE)
        {
            return new NoInputState;
        }
    }

    if (t_event == Event::EVENT_HIT_L)
    {
        return new DamageLState;
    }

    return nullptr;
}

void CollisionRState::update(GameObject* obj)
{
    m_tick++;
    obj->animation(COLLIDE_R);
}

void CollisionRState::enter(GameObject* obj)
{
    m_tick = 0;
    obj->resetAnimation();
}

void CollisionRState::exit(GameObject* obj)
{
}