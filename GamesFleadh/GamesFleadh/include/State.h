#pragma once
#include <raylib.h>
#include "events.h"

enum StateIndex{DEFAULT, ATTACK, DEATH, DAMAGE, ALERT, CHASE, SHOOT, HIT_L, HIT_R, COLLIDE_L, COLLIDE_R, IDLE_1, IDLE_2, DASH};

class GameObject;
class MovingUpState;

class State {
public:
	virtual ~State() {}
	virtual State* handleInput(Event t_event) = 0;
	virtual void update(GameObject* obj) = 0;
	virtual void enter(GameObject* obj) = 0;
	virtual void exit(GameObject* obj) = 0;

	State() = default; // This is bad and temporary.
};



