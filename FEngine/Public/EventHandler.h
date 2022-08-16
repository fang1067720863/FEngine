#pragma once


#include"Event.h"
#include<Camera.h>
#include<iostream>


class FlyCameraController : public EventHandler
{
public:
	FlyCameraController(Ptr<FCamera> camera):_camera(camera){}

	Ptr<FCamera> _camera;
	bool Handle(const Event& event) {return false;}
	virtual bool Handle(const KeyPressEvent& event) {
	    
		switch (event.keySymbol)
		{
		case KeySymbol::KEY_W:
		case KeySymbol::KEY_w :
			_camera->Walk(0.1);
			break;
		case KeySymbol::KEY_S:
		case KeySymbol::KEY_s:
			_camera->Walk(-0.1);
			break;
		case KeySymbol::KEY_A:
		case KeySymbol::KEY_a:
			_camera->Strafe(-0.1);
			break;
		case KeySymbol::KEY_D:
		case KeySymbol::KEY_d:
			_camera->Strafe(0.1);
			break;
		case KeySymbol::KEY_Q:
		case KeySymbol::KEY_q:
			_camera->GoUp(-0.1);
			break;
		case KeySymbol::KEY_E:
		case KeySymbol::KEY_e:
			_camera->GoUp(0.1);
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