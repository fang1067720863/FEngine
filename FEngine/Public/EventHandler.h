#pragma once


#include"Event.h"
#include<Camera.h>
#include<iostream>




class FlyCameraController : public EventHandler
{
public:
	FlyCameraController(Ptr<FCamera> camera):_camera(camera){}

	Ptr<FCamera> _camera;
	bool Handle(const Event& event) {

		std::cout << "1111"; return false;
	}
	virtual bool Handle(const KeyPressEvent& event) {
	    
		std::cout << "ssssss";
		switch (event.keySymbol)
		{
		case KeySymbol::KEY_W:
		case KeySymbol::KEY_w :
			_camera->Strafe(0.1);
			break;
		default:
			break;
		}
		return false;
	
	}
};

class TrackBallManipulator : public EventHandler
{
	// todo  weakPtr
public:
	Ptr<FCamera> _camera;
	Ptr<FNode> _target;
};