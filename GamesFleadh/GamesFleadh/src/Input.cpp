#include "Input.h"

Input* Input::m_instance = nullptr;

Input::Input()
{
}

Input* Input::getInstance()
{
	if (m_instance == nullptr) {
		m_instance = new Input();
	}
	return m_instance;
}

Command* Input::handleInput()
{
    if (IsKeyDown(KEY_UP))
    {
        return new MovingCommand();
    }
    if (IsKeyDown(KEY_DOWN))
    {
        return new MovingCommand();
    }
    if (IsKeyDown(KEY_LEFT))
    {
        return new MovingCommand();
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        return new MovingCommand();
    }
}
