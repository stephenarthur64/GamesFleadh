#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "mediator.h"

// For a good explanation on Mediator Pattern see
// https://www.geeksforgeeks.org/mediator-design-pattern/

// Creates and initializes a new mediator instance
Mediator *CreateMediator(GameObject *obj)
{
    Mediator *mediator = (Mediator *)malloc(sizeof(Mediator));
    mediator->obj = obj;
    return mediator;
}

// Function to execute a command through the mediator
void MediatorExecuteCommand(Command command, Mediator *mediator)
{
    switch (command)
    {
    case COMMAND_NONE:
        HandleEvent(mediator->obj, EVENT_NONE);
        break;
    case COMMAND_MOVE_UP:
        HandleEvent(mediator->obj, EVENT_MOVE);
        break;
    case COMMAND_MOVE_DOWN:
        HandleEvent(mediator->obj, EVENT_MOVE);
        break;
    case COMMAND_MOVE_LEFT:
        HandleEvent(mediator->obj, EVENT_MOVE);
        break;
    case COMMAND_MOVE_RIGHT:
        HandleEvent(mediator->obj, EVENT_MOVE);
        break;
    case COMMAND_FIRE:
        HandleEvent(mediator->obj, EVENT_ATTACK);
        break;
    case COMMAND_COLLISION_START:
        HandleEvent(mediator->obj, EVENT_DIE);
        break;
    case COMMAND_COLLISION_END:
        HandleEvent(mediator->obj, EVENT_RESPAWN);
        break;
    default:
        break;
    }
}
