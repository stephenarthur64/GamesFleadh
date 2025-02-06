#pragma once

// Forward declaration of the Mediator struct
typedef struct Mediator Mediator;

// Define the Command enum
typedef enum
{
    COMMAND_MOVE_UP,         // Command to move up
    COMMAND_MOVE_UP_RIGHT,   // Command to move diagonally up and right (Not Implemented)
    COMMAND_MOVE_UP_LEFT,    // Command to move diagonally up and left (Not Implemented)
    COMMAND_MOVE_DOWN,       // Command to move down
    COMMAND_MOVE_DOWN_LEFT,  // Command to move diagonally down and left (Not Implemented)
    COMMAND_MOVE_DOWN_RIGHT, // Command to move diagonally down and right (Not Implemented)
    COMMAND_MOVE_LEFT,       // Command to move left
    COMMAND_MOVE_RIGHT,      // Command to move right
    COMMAND_FIRE,            // Command to fire an action (e.g., shoot)
    COMMAND_COLLISION_START, // Command indicating the start of a collision
    COMMAND_COLLISION_END,   // Command indicating the end of a collision
    COMMAND_NONE,            // No command (used to represent a neutral or idle state)
    COMMAND_COUNT            // Total number of commands, useful for looping or limits
} Command;

// Function to execute a command
void ExecuteCommand(Command command, Mediator* mediator);

