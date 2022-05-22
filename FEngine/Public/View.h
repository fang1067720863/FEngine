#pragma once

#include"Ptr.h"
#include"Scene.h"
#include"Camera.h"
#include"FRenderer.h"
#include"FEventQueue.h"
#include"FEventHandler.h"


class FFrameStamp;
class FGUIEventHandler;
class FEventQueue;
class FViewport
{
public:
	int _width;
	int _height;
	inline double aspectRatio() const { if (_height != 0) return (double)_width / (double)_height; else return 1.0; }
};

class FView
{
public:
	FView() {
	}
	void RequestRedraw() {
		_dirtyMask = true;
	}

	FGUIEventQueue* GetEventQueue() {
		return nullptr;
	}
protected:
	FPtr<FCamera>            _camera;
	FPtr<FScene>             _scene;
	FRenderer*               _renderer;
	FViewport                _viewport;

	bool                     _dirtyMask;

	FPtr<FGUIEventHandler>      _eventHandler;
	FPtr<FGUIEventQueue>        _eventQueue;

	void EventUpdate()
	{
		FEventQueue::Events events;
		_eventQueue->TakesEvents(events);
		for(auto iter=events.begin();iter!=events.end();iter++)
		{
			_eventHandler->handle(*iter);
		}
		
	}

	void  Update();

	void Draw();
	
private:
	
	
};

