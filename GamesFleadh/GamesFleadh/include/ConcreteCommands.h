#pragma once

#include "Command.h"

class IdleCommand : public Command
{
	virtual void execute(GameObject * obj) override
	{
		Event event = EVENT_NONE;
		obj->handleInput(event);
	}
};

class MovingUpCommand : public Command
{
	virtual void execute(GameObject* obj) override
	{
		Event event = EVENT_MOVE_UP;
		obj->handleInput(event);
	}
};

class MovingDownCommand : public Command
{
	virtual void execute(GameObject* obj) override
	{
		Event event = EVENT_MOVE_DOWN;
		obj->handleInput(event);
	}
};

class MovingLeftCommand : public Command
{
	virtual void execute(GameObject* obj) override
	{
		Event event = EVENT_MOVE_LEFT;
		obj->handleInput(event);
	}
};

class MovingRightCommand : public Command
{
	virtual void execute(GameObject* obj) override
	{
		Event event = EVENT_MOVE_RIGHT;
		obj->handleInput(event);
	}
};

class ShootCommand : public Command
{
	virtual void execute(GameObject* obj) override
	{
		Event event = EVENT_SHOOT;
		obj->handleInput(event);
	}
};