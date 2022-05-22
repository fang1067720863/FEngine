#pragma once

#include"Matrix.h"

template<typename T>
class Quat
{
    using value_type = T;

    union
    {
        value_type value[4];
        struct
        {
            value_type x, y, z, w;
        };
    };

    constexpr Quat() :
        value{} {}
    constexpr Quat(const Quat& v) :
        value{ v.x, v.y, v.z, v.w } {}
    constexpr Quat(value_type in_x, value_type in_y, value_type in_z, value_type in_w) :
        value{ in_x, in_y, in_z, in_w } {}
    constexpr Quat(value_type angle_radians, const t_vec3<value_type>& axis)
    {
        set(angle_radians, axis);
    }
    constexpr Quat(const t_vec3<value_type>& from, const t_vec3<value_type>& to)
    {
        set(from, to);
    }

    template<typename R>
    constexpr explicit Quat(const Quat<R>& v) :
        value{ static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z), static_cast<T>(v.w) } {}

    constexpr Quat& operator=(const Quat&) = default;

    constexpr std::size_t size() const { return 4; }

    value_type& operator[](std::size_t i) { return value[i]; }
    value_type operator[](std::size_t i) const { return value[i]; }

    template<typename R>
    Quat& operator=(const Quat<R>& rhs)
    {
        value[0] = static_cast<value_type>(rhs[0]);
        value[1] = static_cast<value_type>(rhs[1]);
        value[2] = static_cast<value_type>(rhs[2]);
        value[3] = static_cast<value_type>(rhs[3]);
        return *this;
    }

    T* data() { return value; }
    const T* data() const { return value; }

    template<typename T>
    inline Vec4<T> asVec4() const
    {
        return Vec4<T>(_v[0], _v[1], _v[2], _v[3]);
    }

    void set(value_type in_x, value_type in_y, value_type in_z, value_type in_w)
    {
        x = in_x;
        y = in_y;
        z = in_z;
        w = in_w;
    }

    void set(value_type angle_radians, const t_vec3<value_type>& axis)
    {
        const value_type epsilon = 1e-7;
        value_type len = length(axis);
        if (len < epsilon)
        {
            // ~zero length axis, so reset rotation to zero.
            *this = {};
            return;
        }

        value_type inversenorm = 1.0 / len;
        value_type coshalfangle = cos(0.5 * angle_radians);
        value_type sinhalfangle = sin(0.5 * angle_radians);

        x = axis.x * sinhalfangle * inversenorm;
        y = axis.y * sinhalfangle * inversenorm;
        z = axis.z * sinhalfangle * inversenorm;
        w = coshalfangle;
    }

    void set(const t_vec3<value_type>& from, const t_vec3<value_type>& to)
    {
        const value_type epsilon = 1e-7;

        value_type dot_pd = vsg::dot(from, to);
        value_type div = std::sqrt(length2(from) * length2(to));
        vsg::dvec3 axis;
        if (div - dot_pd < epsilon)
        {
            axis = orthogonal(from);
        }
        else
        {
            axis = cross(from, to);
        }

        value_type len = length(axis);

        double angle_radians = acos(dot_pd / div);

        value_type inversenorm = 1.0 / len;
        value_type coshalfangle = cos(0.5 * angle_radians);
        value_type sinhalfangle = sin(0.5 * angle_radians);

        x = axis.x * sinhalfangle * inversenorm;
        y = axis.y * sinhalfangle * inversenorm;
        z = axis.z * sinhalfangle * inversenorm;
        w = coshalfangle;
    }

    template<typename T>
    constexpr T length const()
    {
        return std::sqrt(value[0] * value[0] + value[1] * value[1] + value[2] * value[2] + value[3] * value[3]);
    }

    void Normalize()
    {
        T inverse_len = static_cast<T>(1.0) / length() ;
        set(value[0] * inverse_len, value[1] * inverse_len, value[2] * inverse_len, value[3] * inverse_len);
    }

    void operator*=(const Quat<T>& q)
    {
       /* T inverse_len = static_cast<T>(1.0) / length();
        set(value[0] * inverse_len, value[1] * inverse_len, value[2] * inverse_len, value[3] * inverse_len);*/
    }
    inline Quat<T>& operator*=(const Quat<T>& rhs)
    {
        value_type x = rhs.value[3] * _v[0] + rhs.value[0] * value[3] + rhs.value[1] * value[2] - rhs.value[2] * value[1];
        value_type y = rhs.value[3] * _v[1] - rhs.value[0] * value[2] + rhs.value[1] * value[3] + rhs.value[2] * value[0];
        value_type z = rhs.value[3] * _v[2] + rhs.value[0] * value[1] - rhs.value[1] * value[0] + rhs.value[2] * value[3];
        value[3] = rhs.value[3] * value[3] - rhs.value[0] * value[0] - rhs.value[1] * value[1] - rhs.value[2] * value[2];

        value[2] = z;
        value[1] = y;
        value[0] = x;

        return (*this);            // enable nesting
    }
   
};

using Quatf = Quat<float>;
using Quatd = Quat<double>;

// Rotate a quaternion by another quaternion
template<typename T>
constexpr Quat<T> operator*(const Quat<T>& lhs, const Quat<T>& rhs)
{
    t_quat<T> q(rhs[3] * lhs[0] + rhs[0] * lhs[3] + rhs[1] * lhs[2] - rhs[2] * lhs[1],
        rhs[3] * lhs[1] - rhs[0] * lhs[2] + rhs[1] * lhs[3] + rhs[2] * lhs[0],
        rhs[3] * lhs[2] + rhs[0] * lhs[1] - rhs[1] * lhs[0] + rhs[2] * lhs[3],
        rhs[3] * lhs[3] - rhs[0] * lhs[0] - rhs[1] * lhs[1] - rhs[2] * lhs[2]);

    return q;
}

// Rotate a vector by a quaternion
template<typename T>
constexpr Vec3<T> operator*(const Quat<T>& q, const Vec3<T>& v)
{
    // nVidia SDK implementation
    t_vec3<T> uv, uuv;
    t_vec3<T> qvec(q[0], q[1], q[2]);
    uv = cross(qvec, v);
    uuv = cross(qvec, uv);
    T two(2.0);
    uv *= (two * q[3]);
    uuv *= two;
    return v + uv + uuv;
}

template<typename T>
constexpr Quat<T> normalize(const Quat<T>& v)
{
    T inverse_len = static_cast<T>(1.0) / length(v);
    return t_quat<T>(v[0] * inverse_len, v[1] * inverse_len, v[2] * inverse_len, v[3] * inverse_len);
}

template<typename T>
constexpr T dot(const Quat<T>& lhs, const Quat<T>& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3] * rhs[3];
}

template<typename T>
constexpr Quat<T> inverse(const Quat<T>& v)
{
    t_quat<T> c = conj(v);
    T inverse_len = static_cast<T>(1.0) / length(v);
    return t_quat<T>(c[0] * inverse_len, c[1] * inverse_len, c[2] * inverse_len, c[3] * inverse_len);
}

template<typename T>
constexpr Quat<T> slerp(const Quat<T>& from, Quat<T> to, T r)
{
    T epsilon = std::numeric_limits<T>::epsilon();
    T one(1.0);

    T cosomega = dot(from, to);
    if (cosomega < 0.0)
    {
        cosomega = -cosomega;
        to.x = -to.x;
        to.y = -to.y;
        to.z = -to.z;
        to.w = -to.w;
    }

    if ((one - cosomega) > epsilon)
    {
        T omega = acos(cosomega);
        T sinomega = sin(omega);
        T scale_from = sin((one - r) * omega) / sinomega;
        T scale_to = sin(r * omega) / sinomega;
        return (from * scale_from) + (to * scale_to);
    }
    else
    {
        // quaternion's are very close so just linearly interpolate
        return (from * (one - r)) + (to * r);
    }
}
