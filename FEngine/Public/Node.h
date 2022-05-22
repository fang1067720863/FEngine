#pragma once
#include"Reference.h"
#include"Matrix.h"
#include"Quat.h"

#include<string>

class FNode:public FObject
{
public:
	FNode(std::string name):mName(name)
	{

	}
	inline void const SetName(const std::string& name) { mName = name; }    //…Ë÷√√˚≥∆
	inline const Vec3f GetPosition() const{ }
	inline const Vec3f GetRotation() const { }
	inline const Vec3f GetScale() const { }

	inline const Vec3f GetLocalPosition() const { }
	//inline const  GetLocalRotation() const { }
	inline const Vec3f GetLocalScale() const { }

	void SetLocalPosition(const Vec3f& pos){}
	void SetLocalScale(const Vec3f& scale) {}
	void SetLocalRotation(const Vec3f& euler) {}


	Mat4 mLocalTransform;
	Mat4 mWorldTransform;
	std::string mName;
};

