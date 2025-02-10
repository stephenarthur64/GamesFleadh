#pragma once
#include "State.h"
#include "MovingState.h"

class IdleState : public State {
public:
	State* handleInput(Event t_event) override
	{
		if (t_event == Event::EVENT_MOVE)
		{
			return new MovingState();
		}
	}
	void update(GameObject* obj) override
	{ // Idle behavior (e.g., standing still) ;
	}
	void enter(GameObject* obj) override
	{ // Enter idle state (e.g., reset idle animation) 
	}
	void exit(GameObject* obj) override
	{ // Exit idle state (e.g., save state or cleanup) 
	}
};
