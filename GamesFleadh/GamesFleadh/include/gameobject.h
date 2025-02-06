#pragma once

#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#include "events.h"
#include "fsm.h"

//// Base structure for a game object
//typedef struct GameObject
//{
//    const char *name;    // The name of the game object (e.g., "Player", "Enemy")
//    
//
//    StateConfig *stateConfigs; // Pointer to the array of state configurations for this game object
//
//    int health; // The health of the game object
//} GameObject;
//
//// Initialize a new game object with the given name and default values
//void InitGameObject(GameObject *obj, const char *name);
//
//// Delete a game object and free associated memory/resources
//void DeleteGameObject(GameObject *obj);

class GameObject {
public:
    State previousState; // The state the game object was previously in
    State currentState;  // The current state of the game object
    StateConfig* stateConfigs; // Pointer to the array of state configurations for this game object

    GameObject() : m_position({ 7.0f, 0.0f, 0.0f }), m_colour(WHITE) {}

    Model* getModel() { return &m_body; }
    Vector3 getPositon() { return m_position; }
    BoundingBox getHitbox() { return m_hitbox; }
    Color getColour() { return m_colour; }

protected:
    Model m_body;
    Vector3 m_position;
    BoundingBox m_hitbox;
    Color m_colour;
};

