#pragma once

// Define an enumeration for different event types
typedef enum
{
    EVENT_NONE,   // Represents no event
    EVENT_MOVE_UP,   // Represents a move event
    EVENT_MOVE_DOWN,
    EVENT_MOVE_LEFT,
    EVENT_MOVE_RIGHT,
    EVENT_MOVE_UPRIGHT,
    EVENT_MOVE_UPLEFT,
    EVENT_MOVE_DOWNRIGHT,
    EVENT_MOVE_DOWNLEFT,
    EVENT_DAMAGE,
    EVENT_SHOOT,
    EVENT_SPOTTED,
    EVENT_DIE,    // Represents a die event
    EVENT_RESPAWN,// Represents a respawn event
    EVENT_COUNT   // Represents the total number of events (for counting purposes)
} Event;          // Define 'Event' as the type of the enum
