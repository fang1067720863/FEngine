#pragma once
#include"Vec.h"
#include"Quat.h"
#include<vector>
template<typename T>
struct Matrix4
{
public:
    using value_type = T;
    using column_type = Vec4<T>;

    column_type value[4];

    constexpr Matrix4() :
        value{ {1, 0, 0, 0},
              {0, 1, 0, 0},
              {0, 0, 1, 0},
              {0, 0, 0, 1} } {}

    constexpr explicit Matrix4(value_type v) :
        value{ {v, 0, 0, 0},
              {0, v, 0, 0},
              {0, 0, v, 0},
              {0, 0, 0, v} } {}

    constexpr Matrix4(value_type v0, value_type v1, value_type v2, value_type v3,   /* column 0 */
        value_type v4, value_type v5, value_type v6, value_type v7,   /* column 1 */
        value_type v8, value_type v9, value_type v10, value_type v11, /* column 2 */
        value_type v12, value_type v13, value_type v14, value_type v15) /* column 3 */ :
        value{ {v0, v1, v2, v3},
              {v4, v5, v6, v7},
              {v8, v9, v10, v11},
              {v12, v13, v14, v15} }
    {
    }

    constexpr explicit Matrix4(value_type v[16]) :
        value{ {v[0], v[1], v[2], v[3]},
              {v[4], v[5], v[6], v[7]},
              {v[8], v[9], v[10], v[11]},
              {v[12], v[13], v[14], v[15]} } {}
    
    constexpr Matrix4(const column_type& c0,
        const column_type& c1,
        const column_type& c2,
        const column_type& c3) :
        value{ c0, c1, c2, c3 }
    {
    }

    template<typename R>
    explicit Matrix4(const Matrix4<R>& rhs)
    {
        value[0] = rhs[0];
        value[1] = rhs[1];
        value[2] = rhs[2];
        value[3] = rhs[3];
    }

    template<typename R>
    constexpr explicit Matrix4(const std::vector<R>& v): 
            value{ {v[0], v[1], v[2], v[3]},
              {v[4], v[5], v[6], v[7]},
              {v[8], v[9], v[10], v[11]},
              {v[12], v[13], v[14], v[15]} } {}
  

    constexpr std::size_t size() const { return 16; }
    constexpr std::size_t columns() const { return 4; }
    constexpr std::size_t rows() const { return 4; }

    column_type& operator[](std::size_t c) { return value[c]; }
    const column_type& operator[](std::size_t c) const { return value[c]; }

    value_type& operator()(std::size_t c, std::size_t r) { return value[c][r]; }
    value_type operator()(std::size_t c, std::size_t r) const { return value[c][r]; }

    template<typename R>
    Matrix4& operator=(const Matrix4<R>& rhs)
    {
        value[0] = rhs[0];
        value[1] = rhs[1];
        value[2] = rhs[2];
        value[3] = rhs[3];
        return *this;
    }

    //template<typename R>
    //Matrix4& operator*=(const Matrix4<R>& rhs)
    //{
    //    (*this) * rhs
    //    value[0] = rhs[0];
    //    value[1] = rhs[1];
    //    value[2] = rhs[2];
    //    value[3] = rhs[3];
    //    return *this;
    //}

    void set(value_type v0, value_type v1, value_type v2, value_type v3,     /* column 0 */
        value_type v4, value_type v5, value_type v6, value_type v7,     /* column 1 */
        value_type v8, value_type v9, value_type v10, value_type v11,   /* column 2 */
        value_type v12, value_type v13, value_type v14, value_type v15) /* column 3 */
    {
        value[0].set(v0, v1, v2, v3);
        value[1].set(v4, v5, v6, v7);
        value[2].set(v8, v9, v10, v11);
        value[3].set(v12, v13, v14, v15);
    }

    void transpose()
    {
        set(value[0][0], value[1][0], value[2][0], value[3][0],
            value[0][1], value[1][1], value[2][1], value[3][1],
            value[0][2], value[1][2], value[2][2], value[3][2],
            value[0][3], value[1][3], value[2][3], value[3][3]);
    }

    


    // todo
    template<typename T>
    void rotate(T angle_radians, T x, T y, T z)
    {
        const T c = std::cos(angle_radians);
        const T s = std::sin(angle_radians);
        const T one_minus_c = 1 - c;
        return set(x * x * one_minus_c + c, y * x * one_minus_c + z * s, x * z * one_minus_c - y * s, 0,
            x * y * one_minus_c - z * s, y * y * one_minus_c + c, y * z * one_minus_c + x * s, 0,
            x * z * one_minus_c + y * s, y * z * one_minus_c - x * s, z * z * one_minus_c + c, 0,
            0, 0, 0, 1);
    }
    T* data() { return value[0].data(); }
    const T* data() const { return value[0].data(); }
};
//
using Mat4 = Matrix4<float>;
using Mat4d = Matrix4<double>;

template<typename T>
T dot(const Matrix4<T>& lhs, const Matrix4<T>& rhs, int c, int r)
{
    return lhs[0][r] * rhs[c][0] +
        lhs[1][r] * rhs[c][1] +
        lhs[2][r] * rhs[c][2] +
        lhs[3][r] * rhs[c][3];
}

template<typename T>
Matrix4<T> operator*(const Matrix4<T>& lhs, const Matrix4<T>& rhs)
{
    return Matrix4<T>(dot(lhs, rhs, 0, 0), dot(lhs, rhs, 0, 1), dot(lhs, rhs, 0, 2), dot(lhs, rhs, 0, 3),
        dot(lhs, rhs, 1, 0), dot(lhs, rhs, 1, 1), dot(lhs, rhs, 1, 2), dot(lhs, rhs, 1, 3),
        dot(lhs, rhs, 2, 0), dot(lhs, rhs, 2, 1), dot(lhs, rhs, 2, 2), dot(lhs, rhs, 2, 3),
        dot(lhs, rhs, 3, 0), dot(lhs, rhs, 3, 1), dot(lhs, rhs, 3, 2), dot(lhs, rhs, 3, 3));
}


template<typename T>
Matrix4<T> rotate(T angle_radians, T x, T y, T z)
{
    const T c = std::cos(angle_radians);
    const T s = std::sin(angle_radians);
    const T one_minus_c = 1 - c;
    return Matrix4<T>(x * x * one_minus_c + c, y * x * one_minus_c + z * s, x * z * one_minus_c - y * s, 0,
        x * y * one_minus_c - z * s, y * y * one_minus_c + c, y * z * one_minus_c + x * s, 0,
        x * z * one_minus_c + y * s, y * z * one_minus_c - x * s, z * z * one_minus_c + c, 0,
        0, 0, 0, 1);
}
template<typename T>
constexpr Matrix4<T> rotate(const Quat<T>& q)
{
    T qxx(q.x * q.x);
    T qyy(q.y * q.y);
    T qzz(q.z * q.z);
    T qxy(q.x * q.y);
    T qxz(q.x * q.z);
    T qyz(q.y * q.z);
    T qwx(q.w * q.x);
    T qwy(q.w * q.y);
    T qwz(q.w * q.z);

    T zero(0.0);
    T one(1.0);
    T two(2.0);

    return Matrix4<T>(one - two * (qyy + qzz), two * (qxy + qwz), two * (qxz - qwy), zero,
        two * (qxy - qwz), one - two * (qxx + qzz), two * (qyz + qwx), zero,
        two * (qxz + qwy), two * (qyz - qwx), one - two * (qxx + qyy), zero,
        zero, zero, zero, 1.0);
}
template<typename T>
Matrix4<T> rotate(T angle_radians, const Vec3<T>& v)
{
    return rotate(angle_radians, v.value[0], v.value[1], v.value[2]);
}

template<typename T>
constexpr Matrix4<T> translate(T x, T y, T z)
{
    return Matrix4<T>(1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1);
}

template<typename T>
constexpr Matrix4<T> translate(const Vec3<T>& v)
{
    return translate(v[0], v[1], v[2]);
}

template<typename T>
constexpr Matrix4<T> scale(T sx, T sy, T sz)
{
    return Matrix4<T>(sx, 0, 0, 0,
        0, sy, 0, 0,
        0, 0, sz, 0,
        0, 0, 0, 1);
}

template<typename T>
constexpr Matrix4<T> scale(const Vec3<T>& v)
{
    return scale(v[0], v[1], v[2]);
}
