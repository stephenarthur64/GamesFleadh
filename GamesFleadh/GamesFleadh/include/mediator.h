#pragma once

#include <stdbool.h>

#include "command.h"
#include "gameobject.h"

// Define the Mediator structure
typedef struct Mediator
{
    GameObject *obj;
} Mediator;

// Function to create a mediator instance
Mediator *CreateMediator(GameObject *obj);

// Execute Command
void MediatorExecuteCommand(Command command, Mediator *mediator);
