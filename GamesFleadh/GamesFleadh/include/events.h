#pragma once

// Define an enumeration for different event types
typedef enum
{
    EVENT_NONE,   // Represents no event
    EVENT_MOVE,   // Represents a move event
    EVENT_DIE,    // Represents a die event
    EVENT_RESPAWN,// Represents a respawn event
    EVENT_COUNT   // Represents the total number of events (for counting purposes)
} Event;          // Define 'Event' as the type of the enum
