#pragma once
#include"Reference.h"
#include"Matrix.h"
#include"Quat.h"

#include<string>
#include<functional>

class FNode:public FObject
{
	using NodeUpdateCallback = std::function<void(float dt)>;
public:
	FNode() :mName("")
	{

	}
	FNode(const std::string& name):mName(name)
	{

	}
	inline void const SetName(const std::string& name) { mName = name; }    //…Ë÷√√˚≥∆
	inline const Vec3f GetLocalPosition() const { return mPosition; }
	inline const Vec3f GetLocalScale() const { return mScale; }
	inline const Mat4 GetWorldTransform() const { return mWorldTransform; }
	inline void rotate(float angle_radians, float x, float y, float z) {
		mWorldTransform.rotate(angle_radians, x, y, z);
	}

	void SetLocalPosition(const Vec3f& pos) { mPosition = pos; dirty = true; }
	void SetLocalScale(const Vec3f& scale) { dirty = true; }
	//void SetLocalRotation(const Vec3f& euler) { mRotation = Quatf(); dirty = true;}
	void Update(float dt)
	{
		if (dirty)
		{

		}
		if (updateCB)
		{
			updateCB(dt);
		}
	}

	void SetUpdateCallback(NodeUpdateCallback cb) { updateCB = cb; }

private:
	Mat4 mLocalTransform;
	Mat4 mWorldTransform;
	Vec3f mPosition;
	Vec3f mScale;
	Quatf mRotation;
	bool dirty;
	std::string mName;
	NodeUpdateCallback updateCB;
};

