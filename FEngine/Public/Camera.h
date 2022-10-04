#pragma once

#include <windows.h>
#include"Node.h"
#include"Matrix.h"
#include<memory>
#include<vector>
#include<wrl/client.h>
#include<DirectXMath.h>



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
        mProjMatrix = perspective<float>(fovy_radians, aspectRatio, zNear, zFar);
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
   // int32_t dirtyMask;


    template<class T>
    constexpr Matrix4<T> perspective(T fovy_radians, T aspectRatio, T zNear, T zFar)
    {
        T f = static_cast<T>(1.0 / std::tan(fovy_radians * 0.5));
        T r = static_cast<T>(1.0 / (zFar - zNear));
      /*  vulkan_perspective_matrix = return Matrix4<T>(f / aspectRatio, 0, 0, 0,
            0, -f, 0, 0,
            0, 0, zNear * r, -1,
            0, 0, (zFar * zNear) * r, 0);*/
      /*  dx_perspective_matrix = return Matrix4<T>(f / aspectRatio, 0, 0, 0,
            0, f, 0, 0,
            0, 0, zFar * r, 1,
            0, 0, -(zFar * zNear) * r, 0);*/
        // dx_perspective_matrix_transpose
        return Matrix4<T>(f / aspectRatio, 0, 0, 0,
            0, f, 0, 0,
            0, 0, zFar * r, -(zFar * zNear) * r,
            0, 0, 1, 1);
    }


    template<typename T>
    constexpr Matrix4<T> orthographic(T left, T right, T bottom, T top, T zNear, T zFar)
    {
        return Matrix4<T>(2.0 / (right - left), 0.0, 0.0, 0.0,
            0.0, 2.0 / (bottom - top), 0.0, 0.0,
            0.0, 0.0, 1.0 / (zFar - zNear), 0.0,
            -(right + left) / (right - left), -(bottom + top) / (bottom - top), zFar / (zFar - zNear), 1.0);
    }



};

