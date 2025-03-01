#pragma once
#include "State.h"
class NoInputState :
    public State
{
public:
	State* handleInput(Event t_event) override;
	void update(GameObject* obj) override;
	void enter(GameObject* obj) override;
	void exit(GameObject* obj) override;

private:
	int tick = 0;
	int randNum = IDLE_1;
};

