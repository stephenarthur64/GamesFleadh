#include <stdbool.h>

#include "command.h"
#include "mediator.h"

// Function to execute a command
void ExecuteCommand(Command command, Mediator *mediator)
{
    MediatorExecuteCommand(command, mediator);
}
