#include "CollisionLState.h"
#include "IdleState.h"
#include "ShootState.h"
#include "DamageRState.h"
#include "NoInputState.h"

State* CollisionLState::handleInput(Event t_event)
{
    if (m_tick >= 62)
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

    if (t_event == Event::EVENT_HIT_R)
    {
        return new DamageRState;
    }

    return nullptr;
}

void CollisionLState::update(GameObject* obj)
{
    m_tick++;
    obj->animation(COLLIDE_L);
}

void CollisionLState::enter(GameObject* obj)
{
    m_tick = 0;
    obj->resetAnimation();
}

void CollisionLState::exit(GameObject* obj)
{
}
