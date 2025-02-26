#pragma once
#include "State.h"
class DamageRState :
    public State
{
public:
	State* handleInput(Event t_event) override;
	void update(GameObject* obj) override;
	void enter(GameObject* obj) override;
	void exit(GameObject* obj) override;

private:
	int m_tick = 0;
};

