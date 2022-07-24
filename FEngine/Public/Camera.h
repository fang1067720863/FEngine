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
public:
    FCamera(const Frustum& frustum, const std::string& name):FNode(name)
    {
        SetFrustum(frustum);
    }
    Mat4 mViewMatrix;
    Vec3f mEye;
    Vec3f mForward;
    Vec3f mUp;

    Mat4 mProjMatrix;

    template<class T>
    constexpr void  lookAt(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up)
    {
        using vec_type = Vec3<T>;

        vec_type forward = normalize(center - eye);
        vec_type up_normal = normalize(up);
        vec_type side = normalize(cross(forward, up_normal));
        vec_type u = normalize(cross(side, forward));
        mEye = eye;
        mForward = forward;
        mUp = up;
       
        float x1 = -eye * side;
        float x2 = -eye * u;
        float x3 = -eye * forward;
        mViewMatrix.set
            (side[0], u[0], -forward[0], 0,
                side[1], u[1], -forward[1], 0,
                side[2], u[2], -forward[2], 0,
                x1, x2, x3, 1);
        mViewMatrix.transpose();
    }

    void Walk(float dt)
    {
        Vec3f newPos = mEye + mForward * dt;
        lookAt(newPos, newPos + mForward, mUp);
    }
    void Strafe(float dt)
    {
        Vec3f side = normalize <float> (cross<float>(mForward, mUp));
        Vec3f newPos = mEye + side * dt;
        lookAt(newPos, newPos + mForward, mUp);
    }
    void SetFrustum(const Frustum& frustum)
    {
        SetFrustum(frustum.mFovY, frustum.mAspect, frustum.mNearZ, frustum.mFarZ);
      
        DirectX::XMMATRIX  a = DirectX::XMMatrixPerspectiveFovLH(frustum.mFovY, frustum.mAspect, frustum.mNearZ, frustum.mFarZ);
        DirectX::XMFLOAT4X4 res;
        XMStoreFloat4x4(&res, a);
    }
    void SetFrustum(float fovy_radians, float aspectRatio, float zNear, float zFar)
    {
        mProjMatrix = perspective<float>(fovy_radians, aspectRatio, zNear, zFar);
    } 

    const Mat4& GetViewMatrix()const { return mViewMatrix; }
    const Mat4& GetProjMatrix()const { return mProjMatrix; }
    const Vec3f& GetEyePos()const { return mEye; }

    template<class T>
    constexpr Matrix4<T> perspective(T fovy_radians, T aspectRatio, T zNear, T zFar)
    {
        T f = static_cast<T>(1.0 / std::tan(fovy_radians * 0.5));
        T r = static_cast<T>(1.0 / (zFar - zNear));
      /*  return Matrix4<T>(f / aspectRatio, 0, 0, 0,
            0, -f, 0, 0,
            0, 0, zNear * r, -1,
            0, 0, (zFar * zNear) * r, 0);*/
      /*  return Matrix4<T>(f / aspectRatio, 0, 0, 0,
            0, f, 0, 0,
            0, 0, zFar * r, 1,
            0, 0, -(zFar * zNear) * r, 0);*/
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

