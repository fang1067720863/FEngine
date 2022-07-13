#pragma once
#include"Reference.h"
#include"Matrix.h"
#include"Quat.h"

#include<string>

class FNode:public FObject
{
public:
	FNode() :mName("")
	{

	}
	FNode(const std::string& name):mName(name)
	{

	}
	inline void const SetName(const std::string& name) { mName = name; }    //…Ë÷√√˚≥∆
	/*inline const Vec3f GetPosition() const{ return }
	inline const Vec3f GetRotation() const { }
	inline const Vec3f GetScale() const { }*/

	inline const Vec3f GetLocalPosition() const { return mPosition; }
	//inline const Quatf GetLocalRotation() const { return mRotation; }
	inline const Vec3f GetLocalScale() const { return mScale; }

	void SetLocalPosition(const Vec3f& pos) { mPosition = pos; dirty = true; }
	void SetLocalScale(const Vec3f& scale) { dirty = true; }
	//void SetLocalRotation(const Vec3f& euler) { mRotation = Quatf(); dirty = true;}
	void Update()
	{
		if (dirty)
		{

		}
	}


	Mat4 mLocalTransform;
	Mat4 mWorldTransform;
	Vec3f mPosition;
	Vec3f mScale;
	Quatf mRotation;
	bool dirty;
	std::string mName;
};

