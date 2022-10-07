#pragma once

#include"Event.h"
#include"Camera.h"


class CameraController : public EventHandler
{
public:
	CameraController(Ptr<FCamera> camera) :_camera(camera)
	{

	}
protected:
	
	virtual Mat4 GetInverseMatrix()   // ViewMatrix
	{
		return translate(-_eye) * rotate(inverse(_rotation));
	}

	virtual Mat4 GetMatrix()       // ModelMatrix of Camera
	{
		return rotate(_rotation) * translate(_eye);
	}

	virtual void UpdateCamera()
	{
		_camera->SetViewMatrix(GetInverseMatrix(), GetMatrix(), _eye);
	}
	// todo  weakPtr
	Ptr<FCamera> _camera;
	Quatf _rotation;
	Vec3f _eye;
};
class TpsCameraController : public CameraController
{
public:
	TpsCameraController(Ptr<FCamera> camera,Ptr<FNode> target, const Vec3f &delta = Vec3f(-2.0f,2.0f,0.0)) :CameraController(camera),_target(target), _delta(delta)
	{
		_distance = delta.length();
		_eye = _target->GetPosition() + delta;
		_rotation = Mat4::lookAt(_localUp, _eye, _target->GetPosition()).GetRotate();
		_rotation = inverse(_rotation);
		Update();
	}

	virtual bool Handle(const MouseScrollEvent& event) 
	{ 
		float delta = (event._motion == ScrollingMotion::SCROLL_UP ? -1.0f : 1.0f) * _sensitivity;
		Approach(delta);
		return false; 
	}
	virtual bool Handle(const MouseDragEvent& event)
	{ 
		double pitch = 0.005 * (event._in_x < 0 ? -1.0 : 1.0);
		double yaw = 0.005 * (event._in_y < 0 ? -1.0 : 1.0);
		Rotate(pitch, yaw);
		return false;
	}

	void Update()
	{
		Mat4 r = rotate(inverse(_rotation));
		Vec3f forward(r[0][2], r[1][2], r[1][3]);
		_eye = _target->GetPosition()+forward * (-_distance);

		_camera->SetViewMatrix(GetInverseMatrix(), GetMatrix(), _eye); 
	}

	void Approach(float delta)
	{
		if ((delta< 0.0f&&_distance<_minRange) || (delta>0.0f&&_distance >_maxRange))
		{
			return;
		}
		_distance += delta;
	}

	void Rotate(float radX, float radY)
	{
		Quatf rotateYaw;
		Quatf rotatePitch;

		Vec3f cameraRight(_rotation * Vec3f(1.0f, 0.0f, 0.0f));

		rotatePitch.makeRotate(radY * 0.8, cameraRight);
		rotateYaw.makeRotate(radX * 0.8, Vec3f(0.0f, 1.0f, 0.0f));

		Quatf newRotation = _rotation * rotateYaw * rotatePitch;
		Mat4 r = rotate(newRotation);
		Vec3f forward(r[0][2], r[1][2], r[1][3]);
		_eye = forward * (-_distance);
		_rotation = newRotation;
	}



	Ptr<FNode> _target;
	Vec3f _delta;
	float _distance;

	float _minRange{ 1.0f };
	float _maxRange{ 6.0f };
	float _sensitivity{ 0.1f };
	Vec3f _localUp{ 0.0f, 1.0f, 0.0f };
	

};

class PawnController : public EventHandler
{
public:
	PawnController(Ptr<FNode> target):_target(target),_speed(0.1f)
	{
		_pos = target->GetPosition();
		_rotation = target->GetRotation();
	}
	virtual bool Handle(const KeyPressEvent& event) {

		switch (event.keySymbol)
		{
		case KeySymbol::KEY_Up:
			WalkFront(0.1);
			break;
		case KeySymbol::KEY_Down:
			WalkBack(-0.1);
			break;
		case KeySymbol::KEY_Left:
			WalkLeft(-0.1);
			break;
		case KeySymbol::KEY_Right:
			WalkRight(-0.1);
			break;
		default:
			break;
		}
		return false;

	}

protected:
	void WalkLeft(float delta)
	{
		_velocity = Vec3f(0.0f, 0.0f, 1.0f);
	}
	void WalkRight(float delta)
	{
		_velocity = Vec3f(0.0f, 0.0f, -1.0f);
	}
	void WalkBack(float delta)
	{
		_velocity = Vec3f(-1.0f, 0.0f, 0.0f);
	}

	void WalkFront(float delta)
	{
		_velocity = Vec3f(1.0f, 0.0f, 0.0f);
	}

	void Update()override
	{
		Vec3f delta = _velocity * _speed;
		_pos += delta;
		_target->SetPosition(_pos);
		_velocity = Vec3f(0.0f, 0.0f, 0.0f);
	}
	Vec3f _pos;
	Quatf _rotation;
	float _speed;
	Vec3f _velocity;

	Ptr<FNode> _target;
};