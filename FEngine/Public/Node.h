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
	

	inline void SetRotate(float angle_radians, float x, float y, float z) {
		mRotation.makeRotate(angle_radians, x, y, z);
		dirty = true;
	}

	void Update(float dt)
	{
		if (dirty)
		{
			mLocalTransform = translate(mPosition) * rotate(mRotation) * scale(mScale);
			dirty = false;
			
		}
		if (updateCB)
		{
			updateCB(dt);
		}
		
	}

	virtual void Draw()
	{

	}

	void SetUpdateCallback(NodeUpdateCallback cb) { updateCB = cb; }

private:

	PROPERTY(std::string, Name)
	PROPERTY(Mat4, LocalTransform)
	PROPERTY(Vec3f, Position)
	PROPERTY(Vec3f, Scale)
	PROPERTY(Quatf, Rotation)

	//Mat4  mWorldTransform;
	bool  dirty{ true };
	NodeUpdateCallback updateCB;
};

