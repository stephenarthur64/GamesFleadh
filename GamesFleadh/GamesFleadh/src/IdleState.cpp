#include "IdleState.h"
#include "MovingUpState.h"
#include "MovingDownState.h"
#include "MovingLeftState.h"
#include "MovingRightState.h"

State* IdleState::handleInput(Event t_event)
{
	if (t_event == Event::EVENT_MOVE_UP)
	{
		return new MovingUpState;
	}
	if (t_event == Event::EVENT_MOVE_DOWN)
	{
		return new MovingDownState;
	}
	if (t_event == Event::EVENT_MOVE_LEFT)
	{
		return new MovingLeftState;
	}
	if (t_event == Event::EVENT_MOVE_RIGHT)
	{
		return new MovingRightState;
	}
	if (t_event == Event::EVENT_MOVE_DOWN)
	{
		//return new MovingUDownState;
	}

	return nullptr;
}
