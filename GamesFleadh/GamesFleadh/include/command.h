#pragma once

#include "gameobject.h"

class Command
{
public:
	Command() {}
	virtual ~Command() {}
	virtual void execute(GameObject* obj) = 0;
};

