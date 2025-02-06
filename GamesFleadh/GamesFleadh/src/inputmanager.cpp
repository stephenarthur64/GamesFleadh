#include <stdlib.h>
#include <time.h>

#include "inputmanager.h"

void InitInputManager()
{
    // Initialize input sources (keyboard, controller, etc.)
}


Command PollInput()
{
    // Return a random command
    return (Command)(rand() % COMMAND_COUNT);
}

void ExitInputManager()
{
    // Cleanup resources if needed
}
