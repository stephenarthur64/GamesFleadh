#pragma once
#include <raylib.h>

class GameObject;
class MovingState;

class State {
public:
	virtual ~State() {}
	virtual void handleInput(GameObject* obj) = 0;
	virtual void update(GameObject* obj) = 0;
	virtual void enter(GameObject* obj) = 0;
	virtual void exit(GameObject* obj) = 0;

	State() = default; // This is bad and temporary.
};

class IdleState : public State {
public:
	void handleInput(GameObject* obj) override
	{
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

class MovingState : public State {
public:
	void handleInput(GameObject* obj) override
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