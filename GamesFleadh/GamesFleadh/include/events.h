#pragma once

// Define an enumeration for different event types
typedef enum
{
    EVENT_MOVE,   // Represents no event
    EVENT_DAMAGE,
    EVENT_SHOOT,
    EVENT_SPOTTED,
    EVENT_CHASE,
    EVENT_NONE,
    EVENT_INPUT,
    EVENT_HIT_L,
    EVENT_HIT_R,
    EVENT_EAT,
    EVENT_DIE,    // Represents a die event
    EVENT_RESPAWN,// Represents a respawn event
    EVENT_COUNT   // Represents the total number of events (for counting purposes)
} Event;          // Define 'Event' as the type of the enum
