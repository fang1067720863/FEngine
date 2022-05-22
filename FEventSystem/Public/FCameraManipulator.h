#pragma once

#include"FEvent.h"
#include"FEventHandler.h"


class FCamera;
class FCameraManipulator :public  FGUIEventHandler
{
public:

	bool  handle(const FGUIEvent& event) override;

protected:
	bool handleFrame(const FGUIEvent& event);
	bool handleResize(const FGUIEvent& event);
	bool handleDoubleClick(const FGUIEvent& event);
	bool handleClick(const FGUIEvent& event);
	bool handleMouseMove(const FGUIEvent& event);
	
	bool handleKeyUp(const FGUIEvent& event);
	bool handleKeyDown(const FGUIEvent& event);
};