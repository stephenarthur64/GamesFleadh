#pragma once
#include <raylib.h>
#include "events.h"

class GameObject;
class MovingState;

class State {
public:
	virtual ~State() {}
	virtual State* handleInput(Event t_event) = 0;
	virtual void update(GameObject* obj) = 0;
	virtual void enter(GameObject* obj) = 0;
	virtual void exit(GameObject* obj) = 0;

	State() = default; // This is bad and temporary.
};



