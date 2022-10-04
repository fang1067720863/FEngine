#pragma once


#include"Event.h"
#include<Camera.h>
#include<iostream>


class FlyCameraController : public EventHandler
{
public:
	FlyCameraController(Ptr<FCamera> camera):_camera(camera){
		
	}
	void SetHomePosition(const Vec3f& eye, const Vec3f& center, const Vec3f& up = Vec3f(0.0f,1.0f,0.0f))
	{
		Mat4 m(Mat4::lookAt(up, eye, center));
		_originEye = eye;
		_originRotation = inverse(m.GetRotate());

		SetTransform(_originEye, _originRotation);
	}
protected:
	virtual bool Handle(const Event& event) { return false; }
	virtual bool Handle(const KeyPressEvent& event) {

		switch (event.keySymbol)
		{
		case KeySymbol::KEY_W:
		case KeySymbol::KEY_w:
			Walk(0.1);
			break;
		case KeySymbol::KEY_S:
		case KeySymbol::KEY_s:
			Walk(-0.1);
			break;
		case KeySymbol::KEY_A:
		case KeySymbol::KEY_a:
			Strafe(-0.1);
			break;
		case KeySymbol::KEY_D:
		case KeySymbol::KEY_d:
			Strafe(0.1);
			break;
		case KeySymbol::KEY_Q:
		case KeySymbol::KEY_q:
			GoUp(-0.1);
			break;
		case KeySymbol::KEY_E:
		case KeySymbol::KEY_e:
			GoUp(0.1);
			break;
		case KeySymbol::KEY_H:
		case KeySymbol::KEY_Home:
		case KeySymbol::KEY_h:
			Home();
			break;
		default:
			break;
		}
		UpdateCamera();
		return false;

	}
	virtual bool Handle(const MouseScrollEvent& event)
	{
		
		Zoom((event._motion == ScrollingMotion::SCROLL_UP ? 1.0f : -1.0f) * 0.1f);
		UpdateCamera();
		return true;
	}
	virtual bool Handle(const MouseDragEvent& event)
	{
		double pitch = 0.005 * (event._in_x < 0 ? -1.0 : 1.0);
		double yaw = 0.005 * (event._in_y < 0 ? -1.0 : 1.0);
		RotateYawAndPitch(_rotation, pitch, yaw);
		UpdateCamera();
		return false;
	}
	
	void UpdateCamera()
	{
		//_camera->SetViewMatrix(GetInverseMatrix());
		_camera->SetViewMatrix(GetInverseMatrix(), GetMatrix(), _eye);
	}

	virtual Mat4 GetInverseMatrix()   // ViewMatrix
	{
		return translate(-_eye) * rotate(inverse(_rotation));
	}

	virtual Mat4 GetMatrix()       // ModelMatrix of Camera
	{
		return rotate(_rotation) * translate(_eye);
	}

	void SetTransform(const Vec3f& localUp, const Vec3f& eye, const Vec3f& center)
	{
		Mat4 m(Mat4::lookAt(localUp, eye, center));
		_eye = eye;
		_rotation = inverse(m.GetRotate());
	}
	void SetTransform(const Vec3f& eye, const Quatf& rotation)
	{
		_eye = eye;
		_rotation = rotation;
	}
	void Home(){ 
		SetTransform(_originEye, _originRotation); }
	

	void Walk(float distance)
	{
		_eye +=  Vec3f(distance, 0.0f, 0.0f);
	}
	void Strafe(float distance)
	{
		_eye +=  Vec3f(0.0f, 0.0f, distance);
	}
	void GoUp(float distance)
	{
		_eye +=  Vec3f(0.0f, distance, 0.0f);
	}

	void Zoom(float delta)
	{
		_eye +=  Vec3f(delta, 0.0f, 0.0f);
	}



	void RotateYawAndPitch(Quatf &rotation, double yaw, double pitch, const Vec3f& localUp = Vec3f(1.0f))
	{
		// rotations
		Quatf rotateYaw;
		Quatf rotatePitch;
		Quatf newRotation;

		rotateYaw.makeRotate(-yaw, Vec3f(0.0f, 1.0f, 0.0f));

		

		Vec3f cameraRight(rotation * Vec3f(1.0f,0.0f,0.0f));

		double my_dy = pitch;
		int i = 0;

		do {

			// rotations
			rotatePitch.makeRotate(my_dy, cameraRight);
			newRotation = rotation * rotateYaw * rotatePitch;

			// update vertical axis
			/*if (verticalAxisFixed)
				fixVerticalAxis(newRotation, localUp, false);*/

			// check for viewer's up vector to be more than 90 degrees from "up" axis
			Vec3f newCameraUp = newRotation * Vec3f(1.0f, 0.0f, 0.0f);
			if (newCameraUp * localUp > 0.0f)
			{

				// apply new rotation
				rotation = newRotation;
				return;

			}

			my_dy /= 2.;
			if (++i == 20)
			{
				rotation = rotation * rotateYaw;
				return;
			}

		} while (true);
	}
	

private:
	Ptr<FCamera> _camera;
	Quatf _rotation;
	Vec3f _eye;

	Vec3f _originEye;
	Quatf _originRotation;
};

class TrackBallManipulator : public EventHandler
{
	// todo  weakPtr
public:
	Ptr<FCamera> _camera;
	Ptr<FNode> _target;
};