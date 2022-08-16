#pragma once
#include"Reference.h"
#include"Matrix.h"
#include"Quat.h"

#include<string>
#include<functional>

#define PROPERTY(T,var) public: \
inline void Set##var(const T& _var){m##var=_var;}\
T Get##var() const{return m##var;}\
private: T m##var;

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
	inline const Mat4 GetWorldTransform() const { return mWorldTransform; }

	inline void SetRotate(float angle_radians, float x, float y, float z) {
		mWorldTransform.rotate(angle_radians, x, y, z);
	}

	void Update(float dt)
	{
		if (dirty)
		{
			mLocalTransform = translate(mPosition) * rotate(mRotation) * scale(mScale);
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

	Mat4  mWorldTransform;
	bool  dirty{ true };
	NodeUpdateCallback updateCB;
};

