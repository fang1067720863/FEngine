#pragma once

#include"Node.h"
#include"Matrix.h"
struct Frustum
{
    float mNearZ = 0.5f;
    float mFarZ = 1000.0f;
    float mAspect = 0.0f;
    float mFovY = 800/600;
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
    Mat4 mProjMatrix;

    template<class T>
    constexpr void  lookAt(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up)
    {
        using vec_type = Vec3<T>;

        vec_type forward = normalize(center - eye);
        vec_type up_normal = normalize(up);
        vec_type side = normalize(cross(forward, up_normal));
        vec_type u = normalize(cross(side, forward));

        mViewMatrix.set
            (side[0], u[0], -forward[0], 0,
                side[1], u[1], -forward[1], 0,
                side[2], u[2], -forward[2], 0,
                0, 0, 0, 1);
            //*translate(-eye.x, -eye.y, -eye.z);
    }

    void SetFrustum(const Frustum& frustum)
    {
        SetFrustum(frustum.mFovY, frustum.mAspect, frustum.mNearZ, frustum.mFarZ);
    }
    void SetFrustum(float fovy_radians, float aspectRatio, float zNear, float zFar)
    {
        mProjMatrix = perspective<float>(fovy_radians, aspectRatio, zNear, zFar);
    } 

    const Mat4& GetViewMatrix()const { return mViewMatrix; }
    const Mat4& GetProjMatrix()const { return mProjMatrix; }

    template<class T>
    constexpr Matrix4<T> perspective(T fovy_radians, T aspectRatio, T zNear, T zFar)
    {
        T f = static_cast<T>(1.0 / std::tan(fovy_radians * 0.5));
        T r = static_cast<T>(1.0 / (zFar - zNear));
        return Matrix4<T>(f / aspectRatio, 0, 0, 0,
            0, -f, 0, 0,
            0, 0, zNear * r, -1,
            0, 0, (zFar * zNear) * r, 0);
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

