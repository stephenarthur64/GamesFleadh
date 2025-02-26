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
    if (IsKeyDown(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        return new ShootCommand();
    }
    else
    {
        return new IdleCommand();
    }
}
