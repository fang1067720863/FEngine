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

    template<typename R>
    inline R signOrZero(R v) { return v < (R)0 ? (R)-1 : (v > (R)0 ? (R)1 : 0); }

    template<typename R>
    inline R cmax(R lhs, R rhs) { return lhs > rhs ? lhs : rhs; }

    Quat<value_type> GetRotate2()
    {
        Quat<value_type> q;

        // From http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
        value_type QW = 0.5 * std::sqrt(cmax(0.0, 1.0 + value[0][0] + value[1][1] + value[2][2]));
        value_type QX = 0.5 * std::sqrt(cmax(0.0, 1.0 + value[0][0] - value[1][1] - value[2][2]));
        value_type QY = 0.5 * std::sqrt(cmax(0.0, 1.0 - value[0][0] + value[1][1] - value[2][2]));
        value_type QZ = 0.5 * std::sqrt(cmax(0.0, 1.0 - value[0][0] - value[1][1] + value[2][2]));


        QX = QX * signOrZero(value[1][2] - value[2][1]);
        QY = QY * signOrZero(value[2][0] - value[0][2]);
        QZ = QZ * signOrZero(value[0][1] - value[1][0]);

        return Quat<value_type>(QX, QY, QZ, QW);
    }
    
    Quat<value_type> GetRotate3()
    {
        // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
        // article "Quaternion Calculus and Fast Animation".

        value_type fTrace = value[0][0] + value[1][1] + value[2][2];
        value_type fRoot;
        value_type x, y, z, w;
        if (fTrace > 0.0)
        {
            // |w| > 1/2, may as well choose w > 1/2
            fRoot = std::sqrt(fTrace + 1.0f);  // 2w
            w = 0.5f * fRoot;
            fRoot = 0.5f / fRoot;  // 1/(4w)
            x = (value[2][1] - value[1][2]) * fRoot;
            y = (value[0][2] - value[2][0]) * fRoot;
            z = (value[1][0] - value[0][1]) * fRoot;
        }
        else
        {
            // |w| <= 1/2
            static size_t s_iNext[3] = { 1, 2, 0 };
            size_t i = 0;
            if (value[1][1] > value[0][0])
                i = 1;
            if (value[2][2] > value[i][i])
                i = 2;
            size_t j = s_iNext[i];
            size_t k = s_iNext[j];

            fRoot = std::sqrt(value[i][i] - value[j][j] - value[k][k] + 1.0f);
            float* apkQuat[3] = { &x, &y, &z };
            *apkQuat[i] = 0.5f * fRoot;
            fRoot = 0.5f / fRoot;
            w = (value[k][j] - value[j][k]) * fRoot;
            *apkQuat[j] = (value[j][i] + value[i][j]) * fRoot;
            *apkQuat[k] = (value[k][i] + value[i][k]) * fRoot;
        }
        return Quat<value_type>(x,y,z,w);
    }

    Quat<value_type> GetRotate()
    {
        Quat<value_type> q;

        value_type s;
        value_type tq[4];
        int    i, j;

        // Use tq to store the largest trace
        tq[0] = 1 + value[0][0] + value[1][1] + value[2][2];
        tq[1] = 1 + value[0][0] - value[1][1] - value[2][2];
        tq[2] = 1 - value[0][0] + value[1][1] - value[2][2];
        tq[3] = 1 - value[0][0] - value[1][1] + value[2][2];

        // Find the maximum (could also use stacked if's later)
        j = 0;
        for (i = 1; i < 4; i++) j = (tq[i] > tq[j]) ? i : j;

        // check the diagonal
        if (j == 0)
        {
            /* perform instant calculation */
            q[3] = tq[0];
            q[0] = value[1][2] - value[2][1];
            q[1] = value[2][0] - value[0][2];
            q[2] = value[0][1] - value[1][0];
        }
        else if (j == 1)
        {
            q[3] = value[1][2] - value[2][1];
            q[0] = tq[1];
            q[1] = value[0][1] + value[1][0];
            q[2] = value[2][0] + value[0][2];
        }
        else if (j == 2)
        {
            q[3] = value[2][0] - value[0][2];
            q[0] = value[0][1] + value[1][0];
            q[1] = tq[2];
            q[2] = value[1][2] + value[2][1];
        }
        else /* if (j==3) */
        {
            q[3] = value[0][1] - value[1][0];
            q[0] = value[2][0] + value[0][2];
            q[1] = value[1][2] + value[2][1];
            q[2] = tq[3];
        }

        s = sqrt(0.25 / tq[j]);
        q[3] *= s;
        q[0] *= s;
        q[1] *= s;
        q[2] *= s;

        return q;
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
    
    template<typename T>
    static constexpr Matrix4<T> translate(T x, T y, T z)
    {
        return Matrix4<T>(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            x, y, z, 1);
    }

    template<typename T>
    static constexpr Matrix4<T> transpose(const Matrix4<T>& m)
    {
        return Matrix4<T>(m[0][0], m[1][0], m[2][0], m[3][0],
            m[0][1], m[1][1], m[2][1], m[3][1],
            m[0][2], m[1][2], m[2][2], m[3][2],
            m[0][3], m[1][3], m[2][3], m[3][3]);
        
    }

    template<typename T>
    static Matrix4<T> lookAt(const Vec3<T>& localUp, const Vec3<T>& eye, const Vec3<T>& center)
    {
        Matrix4<T> m;
        Vec3<T> f(center - eye);
        f.normalize();
        Vec3<T> s(localUp^f);
        s.normalize();
        Vec3<T> u( f^s);
        u.normalize();

        T x = -eye * s;
        T y = -eye * u;
        T z = -eye * f;

        /*m.set(
            s[0], u[0], -f[0], 0.0,
            s[1], u[1], -f[1], 0.0,
            s[2], u[2], -f[2], 0.0,
            x,y,z, 1.0);*/


         m.set(
                s[0], u[0], f[0], 0.0,
                s[1], u[1], f[1], 0.0,
                s[2], u[2], f[2], 0.0,
                0,0,0, 1.0);
        return translate<T>(-eye[0], -eye[1], -eye[2]) *m;
        return m;

    }
    T* data() { return value[0].data(); }
    const T* data() const { return value[0].data(); }
};
//
using Mat4 = Matrix4<float>;
using Mat4d = Matrix4<double>;

template<typename T>
T dot(const Matrix4<T>& lhs, const Matrix4<T>& rhs, int r, int c)
{
    return lhs[r][0] * rhs[0][c] +
        lhs[r][1] * rhs[1][c] +
        lhs[r][2] * rhs[2][c] +
        lhs[r][3] * rhs[3][c];
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
constexpr Matrix4<T> rotate2(const Quat<T>& q)
{
    Matrix4<T> kRot;

    T x, y, z, w;
    x = q[0]; y = q[1]; z = q[2]; w = q[3];
    T fTx = x + x;
    T fTy = y + y;
    T fTz = z + z;
    T fTwx = fTx * w;
    T fTwy = fTy * w;
    T fTwz = fTz * w;
    T fTxx = fTx * x;
    T fTxy = fTy * x;
    T fTxz = fTz * x;
    T fTyy = fTy * y;
    T fTyz = fTz * y;
    T fTzz = fTz * z;

    kRot[0][0] = 1.0f - (fTyy + fTzz);
    kRot[0][1] = fTxy - fTwz;
    kRot[0][2] = fTxz + fTwy;
    kRot[1][0] = fTxy + fTwz;
    kRot[1][1] = 1.0f - (fTxx + fTzz);
    kRot[1][2] = fTyz - fTwx;
    kRot[2][0] = fTxz - fTwy;
    kRot[2][1] = fTyz + fTwx;
    kRot[2][2] = 1.0f - (fTxx + fTyy);
    return kRot;
}




template<typename T>
Matrix4<T> rotate(T angle_radians, const Vec3<T>& v)
{
    return rotate(angle_radians, v[0], v[1], v[2]);
}

template<typename T>
constexpr Matrix4<T> translate(T x, T y, T z)
{
    // dx_translate 
    return Matrix4<T>(1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x ,y, z, 1);   
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
