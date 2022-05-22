#pragma once

#include<list>
#include"Reference.h"
#include"FEvent.h"
#include"FEventHandler.h"
#include"Ptr.h"

class FEvent;
class FEventQueue:public FReference{
public:
	typedef std::list< FPtr<FEvent> > Events;
    Events _eventQueue;
    FEventQueue() = default;

	bool TakeEvents(Events& events) 
    {
        if (!_eventQueue.empty())
        {
            Events::reverse_iterator ritr = _eventQueue.rbegin();

            if (ritr == _eventQueue.rend()) return false;

            for (Events::iterator itr = _eventQueue.begin();
                itr != ritr.base();
                ++itr)
            {
				// 这里pushback  Event绝不允许重新构造  迭代器 pushback
                events.push_back(*itr);
            }
            
            _eventQueue.erase(_eventQueue.begin(), ritr.base());
            return true;
        }
        else
        {
            return false;
        }
	}

};

class FGUIEventQueue : public FEventQueue {
public:
	void MouseMotion(float mx, int my) {}

    void MouseButtonPress(float mx, float my, int button){}

    void MouseDoubleButtonPress(float mx, float my, int button) {}

    void MouseButtonRelease(float mx, float my, int button) {}

  
    FGUIEvent* MouseScroll(const FGUIEvent::ScrollMotion &sm) {
    
        FGUIEvent* event = new FGUIEvent();
        event->SetEventType(FGUIEvent::EventType::SCROLL);
        event->setScrollingMotion(sm);
        addEvent(event);
        return event;
    }

    void KeyPress(int key){}

    void KeyRelease(int key) {}

    void CloseWindow(){}

    void ResizeWindow(){}

    void QuitApplication(){}

    void Frame(){}

    void addEvent(FEvent* event)
    {
        _eventQueue.push_back(event);
    }

};