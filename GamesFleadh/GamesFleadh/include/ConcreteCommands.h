#pragma once

#include "Command.h"

class IdleCommand : public Command
{
	virtual void execute(GameObject * obj) override
	{
		Event event = EVENT_MOVE;
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

class NoInputCommand : public Command
{
	virtual void execute(GameObject* obj) override
	{
		Event event = EVENT_NONE;
		obj->handleInput(event);
	}
};