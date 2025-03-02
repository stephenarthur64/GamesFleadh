#include "NoInputState.h"
#include "IdleState.h"
#include "ShootState.h"
#include "DamageLState.h"
#include "DamageRState.h"
#include "CollisionLState.h"
#include "CollisionRState.h"
#include "DeathState.h"

State* NoInputState::handleInput(Event t_event)
{
    if (tick > 180)
    {
        if (t_event == EVENT_MOVE)
        {
            return new IdleState;
        }
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

    if (t_event == EVENT_COLLIDE_L)
    {
        return new CollisionLState;
    }
    if (t_event == EVENT_COLLIDE_R)
    {
        return new CollisionRState;
    }

    if (t_event == EVENT_DIE)
    {
        return new DeathState;
    }

    return nullptr;
}

void NoInputState::update(GameObject* obj)
{
    if (tick < 180)
    {
        obj->animation(randNum);
    }
    else
    {
        obj->animation(DEFAULT);
    }
    tick++;

    if (tick > 240)
    {
        obj->resetAnimation();
        tick = 0;
        randNum = (rand() % (IDLE_3 - IDLE_1 + 1)) + IDLE_1;
    }
}

void NoInputState::enter(GameObject* obj)
{
    tick = 0;
    obj->resetAnimation();
}

void NoInputState::exit(GameObject* obj)
{
}
