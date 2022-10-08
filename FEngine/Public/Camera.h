#pragma once

#include <windows.h>
#include<memory>
#include<vector>


#include<wrl/client.h>
#include<DirectXMath.h>

#include"Node.h"
#include"Matrix.h"
#include"Factory.h"




struct Frustum
{
    float mNearZ = 0.5f;
    float mFarZ = 1000.0f;
    float mAspect = 800 / 600;
    float mFovY = (float)PI/3;
    //XM_PI / 3, AspectRatio(), 0.5f, 1000.0f
};

class FCamera: public FNode
{
    enum Projection
    {
        Perspective,
        Orthographic
    };
    PROPERTY_GETTER(Mat4, ViewMatrix)
    PROPERTY_GETTER(Mat4, ProjMatrix)
    PROPERTY_GETTER(Mat4, ViewMatrixInverse)
    PROPERTY_GETTER(Mat4, ProjMatrixInverse)
    PROPERTY_GETTER(Vec3f, EyePos)
    PROPERTY_GETTER(float, ZNear)
    PROPERTY_GETTER(float, ZFar)
public:
    FCamera(const Frustum& frustum, const std::string& name):FNode(name)
    {
        SetFrustum(frustum);
    }
  
    void AddViewUpdateCallback(NodeUpdateCallback cb)
    {
        mViewCallbacks.push_back(std::move(cb));
    }

    void AddProjUpdateCallback(NodeUpdateCallback cb)
    {
        mViewCallbacks.push_back(std::move(cb));
    }

    NodeUpdateCallbackList mViewCallbacks;
    NodeUpdateCallbackList mProjCallbacks;

    void SetFrustum(const Frustum& frustum)
    {
        SetFrustum(frustum.mFovY, frustum.mAspect, frustum.mNearZ, frustum.mFarZ);  
    }
    void SetFrustum(float fovy_radians, float aspectRatio, float zNear, float zFar)
    {
        mZFar = zFar;
        mZNear = zNear;
        mProjMatrix = Mat4::perspective(fovy_radians, aspectRatio, zNear, zFar);
        mProjMatrixInverse = inverse_4x4(mProjMatrix);
    } 

    void SetViewMatrix(const Mat4& mat) { mViewMatrix = mat; mViewMatrix.transpose(); }
    void SetViewMatrix(const Mat4& view, const Mat4& viewInverse, const Vec3f eye) { 
        mViewMatrix = view; 
        mViewMatrix.transpose();
        mViewMatrixInverse = viewInverse;
        mViewMatrixInverse.transpose();
        mEyePos = eye;
    }

    void Update(float dt) override
    {
        for (auto cb : mViewCallbacks)
        {
            cb(dt);
        }
        for (auto cb : mProjCallbacks)
        {
            cb(dt);
        }
    }

};


class CameraManager : public ResourceFactory<Ptr<FCamera>, std::string, CameraManager>
{
public:
    static constexpr const char* MainCamera = "MainCamera";

};