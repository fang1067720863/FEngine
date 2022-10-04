#pragma once
#include"Reference.h"
#include"Matrix.h"
#include"Quat.h"

#include<string>
#include<functional>


enum class RenderMask : uint16_t
{
	None                  = 1 << 0,
    Forward               = 1 << 1,
	GBuffer               = 1 << 2,
	DeferQuad             = 1 << 3,
	Shadow                = 1 << 4,
	Sky                   = 1 << 5,
};

class FNode:public FObject
{
protected:
	using NodeUpdateCallback = std::function<void(float dt)>;
	using NodeUpdateCallbackList = std::vector<NodeUpdateCallback>;
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

	virtual void Update(float dt)
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
	PROPERTY(uint16_t, RenderMask)

	//Mat4  mWorldTransform;
	bool  dirty{ true };
	NodeUpdateCallback updateCB;
};

