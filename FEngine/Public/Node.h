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
	All                   = Forward + GBuffer + DeferQuad+ Shadow+ Sky
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
		mDirtyMask = true;
	}

	virtual void Update(float dt)
	{
		if (mDirtyMask)
		{
			mLocalTransform = translateT(mPosition) * rotate(mRotation)*scale(mScale);
			mDirtyMask = false;
		}
		if (updateCB)
		{
			updateCB(dt);
		}
		
	}

	virtual void Draw(uint16_t mask)
	{
		if (!(mask & mRenderMask)) return;
	}

	void SetUpdateCallback(NodeUpdateCallback cb) { updateCB = cb; }

private:

	PROPERTY(std::string, Name)
	PROPERTY(Mat4, LocalTransform)
	PROPERTY_DIRTY(Vec3f, Position)
	PROPERTY_DIRTY(Vec3f, Scale)
	PROPERTY_DIRTY(Quatf, Rotation)
	PROPERTY_DEFAULT(uint16_t, RenderMask, static_cast<uint16_t>(RenderMask::All))
	PROPERTY_DEFAULT(bool, DirtyMask, true)

	//Mat4  mWorldTransform;

	NodeUpdateCallback updateCB;
};

