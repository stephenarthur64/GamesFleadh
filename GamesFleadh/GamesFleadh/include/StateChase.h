#pragma once
#include "State.h"
#include "gameobject.h"

class StateChase :
    public State
{
public:
	State* handleInput(Event t_event) override;
	void update(GameObject* obj) override;
	void enter(GameObject* obj) override;
	void exit(GameObject* obj) override;
};

