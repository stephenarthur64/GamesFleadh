#ifndef EVENTS_H
#define EVENTS_H

// Define an enumeration for different event types
typedef enum
{
    EVENT_NONE,   // Represents no event
    EVENT_MOVE,   // Represents a move event
    EVENT_ATTACK, // Represents an attack event
    EVENT_DEFEND, // Represents a defend event
    EVENT_DODGE,// Represents a dodge event
    EVENT_CROUCH,// Represents a crouch event
    EVENT_RUN,// Represents a run event
    EVENT_DIE,    // Represents a die event
    EVENT_RESPAWN,// Represents a respawn event
    EVENT_COUNT   // Represents the total number of events (for counting purposes)
} Event;          // Define 'Event' as the type of the enum

#endif