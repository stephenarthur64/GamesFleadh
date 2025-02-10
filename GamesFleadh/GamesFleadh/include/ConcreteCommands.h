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

class MovingCommand : public Command
{
	virtual void execute(GameObject* obj) override
	{
		Event event = EVENT_MOVE;
		obj->handleInput(event);
	}
};