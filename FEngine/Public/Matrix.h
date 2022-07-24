#pragma once
#include"Vec.h"
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

//template<class T>
//bool decompose(const Matrix4<T>& m, Vec3<T>& translation, Quat<T>& rotation, Vec3<T>& scale)
//{
//    // get the translation.
//    translation = m[3].xyz;
//
//    // compute the scale
//    scale[0] = length(m[0].xyz);
//    scale[1] = length(m[1].xyz);
//    scale[2] = length(m[2].xyz);
//
//    // check that we don't have any axis scaled by 0 as this would cause a
//    // divide by zero in the rotation code.
//    if (scale[0] == 0.0 || scale[1] == 0.0 || scale[2] == 0.0) return false;
//
//    // compute rotation matrix and subsequently the quaternion
//    t_mat3<T> rm(m[0].xyz / scale[0],
//        m[1].xyz / scale[1],
//        m[2].xyz / scale[2]);
//
//    auto trace = rm[0][0] + rm[1][1] + rm[2][2]; // diagonal of matrix
//    if (trace > static_cast<T>(0.0))
//    {
//        auto root = sqrt(trace + static_cast<T>(1.0));
//        auto half_inv_root = static_cast<T>(0.5) / root;
//        rotation.set(half_inv_root * (rm[1][2] - rm[2][1]),
//            half_inv_root * (rm[2][0] - rm[0][2]),
//            half_inv_root * (rm[0][1] - rm[1][0]),
//            static_cast<T>(0.5) * root);
//    }
//    else // trace <= 0.0
//    {
//        // locate max on diagonal
//        int i = 0;
//        if (rm[1][1] > rm[0][0]) i = 1;
//        if (rm[2][2] > rm[i][i]) i = 2;
//
//        // set up the orthogonal axis to the max diagonal.
//        int next[3] = { 1, 2, 0 };
//        int j = next[i];
//        int k = next[j];
//
//        auto root = sqrt(rm[i][i] - rm[j][j] - rm[k][k] + static_cast<T>(1.0));
//        auto half_inv_root = static_cast<T>(0.5) / root;
//        rotation[i] = static_cast<T>(0.5) / root;
//        rotation[j] = half_inv_root * (rm[i][j] + rm[j][i]);
//        rotation[k] = half_inv_root * (rm[i][k] + rm[k][i]);
//        rotation[3] = half_inv_root * (rm[j][k] - rm[k][j]);
//    }
//
//    return true;
//}

//template<typename T>
//class TranformMatrix : public Matrix4<T>
//{
//
//    TranformMatrix(const Vec3<T>& position, const Vec3<T>& size, const Quat<T>& rotation):mPosition(position),
//        mScale(size), mRotation(rotation)
//    {
//       
//    }
//private:
//    Vec3<T> mPosition;
//    Vec3<T> mScale;
//    Quat<T> mRotation;
//    bool mDirty = false;
//public:
//
//    template<T>
//    Quat<T> rotation() const
//    {
//        return mRotation;
//    }
//
//    template<T>
//    Vec3<T> position() const
//    {
//        return mPosition;
//    }
//
//    template<T>
//    Vec3<T> size() const
//    {
//        return mScale;
//    }
//
//    template<T>
//    void SetRotation(const Quat<T>& q)
//    {
//        mRotation = q;
//    }
//
//    template<T>
//    void SetPosition(const Vec3<T>& p)
//    {
//        mPosition = p;
//    }
//
//    template<T>
//    void SetScale(const Vec3<T>& s)
//    {
//        mScale = s;
//    }
//
//    template<T>
//    void Rotate(const Quat<T>& q)
//    {
//        mRotation *= q;
//        mDirty = true;
//    }
//
//    template<T>
//    void Rotate(const Vec3<T>& axis, T angle)
//    {
//        Rotate(Quat<T>(angle, axis));
//    }
//
//    template<T>
//    void Rotate(const Vec3<T>& euler)
//    {
//        Rotate(Quat<T>(euler));
//    }
//
//    template<T>
//    void Scale(const Vec3<T>& s)
//    {
//        mScale* = s;
//        mDirty = true;
//    }
//
//    template<T>
//    void Scale(T s)
//    {
//        mScale* = s;
//        mDirty = true;
//    }
//
//    void Tanslate(const Vec3<T>& t)
//    {
//        mPosition += t;
//        mDirty = true;
//    }
//private:
//    void DoCompose()
//    {
//
//    }
//
//
//
//};


//
//VSG_type_name(vsg::mat4);
//VSG_type_name(vsg::dmat4);
//
//template<typename T>
//bool operator==(const t_mat4<T>& lhs, const t_mat4<T>& rhs)
//{
//    return lhs.value[0] == rhs.value[0] &&
//        lhs.value[1] == rhs.value[1] &&
//        lhs.value[2] == rhs.value[2] &&
//        lhs.value[3] == rhs.value[3];
//}
//
//template<typename T>
//bool operator!=(const t_mat4<T>& lhs, const t_mat4<T>& rhs)
//{
//    return lhs.value[0] != rhs.value[0] ||
//        lhs.value[1] != rhs.value[1] ||
//        lhs.value[2] != rhs.value[2] ||
//        lhs.value[3] != rhs.value[3];
//}
//
//template<typename T>
//bool operator<(const t_mat4<T>& lhs, const t_mat4<T>& rhs)
//{
//    if (lhs.value[0] < rhs.value[0]) return true;
//    if (rhs.value[0] < lhs.value[0]) return false;
//    if (lhs.value[1] < rhs.value[1]) return true;
//    if (rhs.value[1] < lhs.value[1]) return false;
//    if (lhs.value[2] < rhs.value[2]) return true;
//    if (rhs.value[2] < lhs.value[2]) return false;
//    return lhs.value[3] < rhs.value[3];
//}
//
//template<typename T>
//T dot(const t_mat4<T>& lhs, const t_mat4<T>& rhs, int c, int r)
//{
//    return lhs[0][r] * rhs[c][0] +
//        lhs[1][r] * rhs[c][1] +
//        lhs[2][r] * rhs[c][2] +
//        lhs[3][r] * rhs[c][3];
//}
//
//template<typename T>
//t_mat4<T> operator*(const t_mat4<T>& lhs, const t_mat4<T>& rhs)
//{
//    return t_mat4<T>(dot(lhs, rhs, 0, 0), dot(lhs, rhs, 0, 1), dot(lhs, rhs, 0, 2), dot(lhs, rhs, 0, 3),
//        dot(lhs, rhs, 1, 0), dot(lhs, rhs, 1, 1), dot(lhs, rhs, 1, 2), dot(lhs, rhs, 1, 3),
//        dot(lhs, rhs, 2, 0), dot(lhs, rhs, 2, 1), dot(lhs, rhs, 2, 2), dot(lhs, rhs, 2, 3),
//        dot(lhs, rhs, 3, 0), dot(lhs, rhs, 3, 1), dot(lhs, rhs, 3, 2), dot(lhs, rhs, 3, 3));
//}
//
//template<typename T>
//t_vec4<T> operator*(const t_mat4<T>& lhs, const t_vec4<T>& rhs)
//{
//    return t_vec4<T>(lhs[0][0] * rhs[0] + lhs[1][0] * rhs[1] + lhs[2][0] * rhs[2] + lhs[3][0] * rhs[3],
//        lhs[0][1] * rhs[0] + lhs[1][1] * rhs[1] + lhs[2][1] * rhs[2] + lhs[3][1] * rhs[3],
//        lhs[0][2] * rhs[0] + lhs[1][2] * rhs[1] + lhs[2][2] * rhs[2] + lhs[3][2] * rhs[3],
//        lhs[0][3] * rhs[0] + lhs[1][3] * rhs[1] + lhs[2][3] * rhs[2] + lhs[3][3] * rhs[3]);
//}
//
///* Right multiplication of a matrix and a plane. This can't be used directly to transform a
//   plane from one coordinate system to another using the coordinate system's matrix; the inverse
//   transpose of the matrix should be used. */
//template<typename T, typename R>
//t_plane<R> operator*(const t_mat4<T>& lhs, const t_plane<R>& rhs)
//{
//    t_plane<R> transformed(lhs[0][0] * rhs[0] + lhs[1][0] * rhs[1] + lhs[2][0] * rhs[2] + lhs[3][0] * rhs[3],
//        lhs[0][1] * rhs[0] + lhs[1][1] * rhs[1] + lhs[2][1] * rhs[2] + lhs[3][1] * rhs[3],
//        lhs[0][2] * rhs[0] + lhs[1][2] * rhs[1] + lhs[2][2] * rhs[2] + lhs[3][2] * rhs[3],
//        lhs[0][3] * rhs[0] + lhs[1][3] * rhs[1] + lhs[2][3] * rhs[2] + lhs[3][3] * rhs[3]);
//    T inv = static_cast<R>(1.0) / length(transformed.n);
//    return t_plane<T>(transformed[0] * inv, transformed[1] * inv, transformed[2] * inv, transformed[3] * inv);
//}
//
///* Left multiplication of a row vector. Equivalent to multiplying the column vector by the
//   matrix transpose. */
//template<typename T>
//t_vec4<T> operator*(const t_vec4<T>& lhs, const t_mat4<T>& rhs)
//{
//    return t_vec4<T>(lhs[0] * rhs[0][0] + lhs[1] * rhs[0][1] + lhs[2] * rhs[0][2] + lhs[3] * rhs[0][3],
//        lhs[0] * rhs[1][0] + lhs[1] * rhs[1][1] + lhs[2] * rhs[1][2] + lhs[3] * rhs[1][3],
//        lhs[0] * rhs[2][0] + lhs[1] * rhs[2][1] + lhs[2] * rhs[2][2] + lhs[3] * rhs[2][3],
//        lhs[0] * rhs[3][0] + lhs[1] * rhs[3][1] + lhs[2] * rhs[3][2] + lhs[3] * rhs[3][3]);
//}
//
///* Left multiplication of a plane and a matrix. This can be used directly to transform a plane
//   from a coordinate system's local coordinates to world coordinates. */
//template<typename T, typename R>
//t_plane<T> operator*(const t_plane<T>& lhs, const t_mat4<R>& rhs)
//{
//    t_plane<T> transformed(lhs[0] * rhs[0][0] + lhs[1] * rhs[0][1] + lhs[2] * rhs[0][2] + lhs[3] * rhs[0][3],
//        lhs[0] * rhs[1][0] + lhs[1] * rhs[1][1] + lhs[2] * rhs[1][2] + lhs[3] * rhs[1][3],
//        lhs[0] * rhs[2][0] + lhs[1] * rhs[2][1] + lhs[2] * rhs[2][2] + lhs[3] * rhs[2][3],
//        lhs[0] * rhs[3][0] + lhs[1] * rhs[3][1] + lhs[2] * rhs[3][2] + lhs[3] * rhs[3][3]);
//    T inv = static_cast<T>(1.0) / length(transformed.n);
//    return t_plane<T>(transformed[0] * inv, transformed[1] * inv, transformed[2] * inv, transformed[3] * inv);
//}
//
//template<typename T>
//t_vec3<T> operator*(const t_mat4<T>& lhs, const t_vec3<T>& rhs)
//{
//    T inv = static_cast<T>(1.0) / (lhs[0][3] * rhs[0] + lhs[1][3] * rhs[1] + lhs[2][3] * rhs[2] + lhs[3][3]);
//    return t_vec3<T>((lhs[0][0] * rhs[0] + lhs[1][0] * rhs[1] + lhs[2][0] * rhs[2] + lhs[3][0]) * inv,
//        (lhs[0][1] * rhs[0] + lhs[1][1] * rhs[1] + lhs[2][1] * rhs[2] + lhs[3][1]) * inv,
//        (lhs[0][2] * rhs[0] + lhs[1][2] * rhs[1] + lhs[2][2] * rhs[2] + lhs[3][2]) * inv);
//}
//
//template<typename T>
//t_vec3<T> operator*(const t_vec3<T>& lhs, const t_mat4<T>& rhs)
//{
//    T inv = static_cast<T>(1.0) / (lhs[0] * rhs[3][0] + lhs[1] * rhs[3][1] + lhs[2] * rhs[3][2] + rhs[3][3]);
//    return t_vec3<T>(lhs[0] * rhs[0][0] + lhs[1] * rhs[0][1] + lhs[2] * rhs[0][2] + rhs[0][3] * inv,
//        lhs[0] * rhs[1][0] + lhs[1] * rhs[1][1] + lhs[2] * rhs[1][2] + rhs[1][3] * inv,
//        lhs[0] * rhs[2][0] + lhs[1] * rhs[2][1] + lhs[2] * rhs[2][2] + rhs[2][3] * inv);
//}