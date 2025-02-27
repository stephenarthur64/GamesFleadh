#pragma once
#include "State.h"
class AttackState :
    public State
{
	State* handleInput(Event t_event) override;
	void update(GameObject* obj) override;
	void enter(GameObject* obj) override;
	void exit(GameObject* obj) override;
};

