#pragma once
#include "ConcreteCommands.h"
#include <raylib.h>

class Input
{
public:
	Input();
	static Input* getInstance();
	Command* handleInput();

private:
	static Input* m_instance;
};

