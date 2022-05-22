#include "..\Public\FCameraManipulator.h"



bool FCameraManipulator::handle(const FGUIEvent& event)
{
	switch (event.getEventType())
	{
	case FGUIEvent::EventType::FRAME:
		return handleFrame(event);
	case FGUIEvent::EventType::DOUBLECLICK:

	default:
		break;
	}
}

bool FCameraManipulator::handleFrame(const FGUIEvent& event)
{

}

bool FCameraManipulator::handleResize(const FGUIEvent& event)
{
	return false;
}

bool FCameraManipulator::handleDoubleClick(const FGUIEvent& event)
{
	return false;
}

bool FCameraManipulator::handleClick(const FGUIEvent& event)
{
	return false;
}

bool FCameraManipulator::handleMouseMove(const FGUIEvent& event)
{
	return false;
}

bool FCameraManipulator::handleKeyUp(const FGUIEvent& event)
{
	return false;
}

bool FCameraManipulator::handleKeyDown(const FGUIEvent& event)
{
	return false;
}
