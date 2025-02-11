#pragma once
#include "State.h"
#include "IdleState.h"
#include "gameobject.h"
#include "Player.h"

class MovingLeftState : public State {
public:
	State* handleInput(Event t_event) override;
	void update(GameObject* obj) override;
	void enter(GameObject* obj) override;
	void exit(GameObject* obj) override;
};
