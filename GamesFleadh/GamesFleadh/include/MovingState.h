#pragma once
#include "State.h"
#include "IdleState.h"

class MovingState : public State {
public:
	State* handleInput(Event t_event) override
	{
	}
	void update(GameObject* obj) override
	{
	}
	void enter(GameObject* obj) override
	{
	}
	void exit(GameObject* obj) override
	{ // Exit idle state (e.g., save state or cleanup) 
	}
};
