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
        return new MovingUpCommand();
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        return new MovingDownCommand();
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        return new MovingLeftCommand();
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        return new MovingRightCommand();
    }
    else if (IsKeyDown(KEY_ENTER))
    {
        return new ShootCommand();
    }
    else
    {
        return new IdleCommand();
    }
}
