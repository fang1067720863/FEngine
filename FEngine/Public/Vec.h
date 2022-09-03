#pragma once

#include"FMath.h"
#include<vector>
template<class T>
class Vec2
{
public:

    /** Data type of vector components.*/
    typedef float scalar_type;

    /** Number of vector components. */
    enum { num_components = 2 };

    /** Vec member variable. */
    scalar_type _v[2];


    /** Constructor that sets all components of the vector to zero */
    Vec2() { _v[0] = 0.0; _v[1] = 0.0; }
    Vec2(scalar_type x, scalar_type y) { _v[0] = x; _v[1] = y; }

    T* data() { return _v; }
    const T* data() const { return _v; }


    inline bool operator == (const Vec2& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1]; }

    inline bool operator != (const Vec2& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1]; }

    inline bool operator <  (const Vec2& v) const
    {
        if (_v[0] < v._v[0]) return true;
        else if (_v[0] > v._v[0]) return false;
        else return (_v[1] < v._v[1]);
    }

    inline scalar_type* ptr() { return _v; }
    inline const scalar_type* ptr() const { return _v; }

    inline void set(scalar_type x, scalar_type y) { _v[0] = x; _v[1] = y; }
    inline void set(const Vec2& rhs) { _v[0] = rhs._v[0]; _v[1] = rhs._v[1]; }

    inline scalar_type& operator [] (int i) { return _v[i]; }
    inline scalar_type operator [] (int i) const { return _v[i]; }

    inline scalar_type& x() { return _v[0]; }
    inline scalar_type& y() { return _v[1]; }

    inline scalar_type x() const { return _v[0]; }
    inline scalar_type y() const { return _v[1]; }

    /** Returns true if all components have values that are not NaN. */
    //inline bool valid() const { return !isNaN(); }

    /** Dot product. */
    inline scalar_type operator * (const Vec2& rhs) const
    {
        return _v[0] * rhs._v[0] + _v[1] * rhs._v[1];
    }

    /** Multiply by scalar. */
    inline const Vec2 operator * (scalar_type rhs) const
    {
        return Vec2(_v[0] * rhs, _v[1] * rhs);
    }

    /** Unary multiply by scalar. */
    inline Vec2& operator *= (scalar_type rhs)
    {
        _v[0] *= rhs;
        _v[1] *= rhs;
        return *this;
    }

    inline Vec2& operator *= (Vec2 rhs)
    {
        _v[0] *= rhs._v[1];
        _v[1] *= rhs._v[2];
        return *this;
    }

    /** Divide by scalar. */
    inline const Vec2 operator / (scalar_type rhs) const
    {
        return Vec2(_v[0] / rhs, _v[1] / rhs);
    }

    /** Unary divide by scalar. */
    inline Vec2& operator /= (scalar_type rhs)
    {
        _v[0] /= rhs;
        _v[1] /= rhs;
        return *this;
    }

    /** Binary vector add. */
    inline const Vec2 operator + (const Vec2& rhs) const
    {
        return Vec2(_v[0] + rhs._v[0], _v[1] + rhs._v[1]);
    }

    /** Unary vector add. Slightly more efficient because no temporary
      * intermediate object.
    */
    inline Vec2& operator += (const Vec2& rhs)
    {
        _v[0] += rhs._v[0];
        _v[1] += rhs._v[1];
        return *this;
    }

    /** Binary vector subtract. */
    inline const Vec2 operator - (const Vec2& rhs) const
    {
        return Vec2(_v[0] - rhs._v[0], _v[1] - rhs._v[1]);
    }

    /** Unary vector subtract. */
    inline Vec2& operator -= (const Vec2& rhs)
    {
        _v[0] -= rhs._v[0];
        _v[1] -= rhs._v[1];
        return *this;
    }

    /** Negation operator. Returns the negative of the Vec2f. */
    inline const Vec2 operator - () const
    {
        return Vec2(-_v[0], -_v[1]);
    }

    /** Length of the vector = sqrt( vec . vec ) */
    inline scalar_type length() const
    {
        return sqrt(_v[0] * _v[0] + _v[1] * _v[1]);
    }

    /** Length squared of the vector = vec . vec */
    inline scalar_type length2(void) const
    {
        return _v[0] * _v[0] + _v[1] * _v[1];
    }

    /** Normalize the vector so that it has length unity.
      * Returns the previous length of the vector.
    */
    inline scalar_type normalize()
    {
        scalar_type norm = Vec2::length();
        if (norm > 0.0)
        {
            scalar_type inv = 1.0f / norm;
            _v[0] *= inv;
            _v[1] *= inv;
        }
        return(norm);
    }

};    // end of class Vec2f
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

/** multiply by vector components. */
template<class T>
inline Vec2<T> componentMultiply(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    return Vec2<T>(lhs[0] * rhs[0], lhs[1] * rhs[1]);
}

template<class T>
inline Vec2<T> componentDivide(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    return Vec2<T>(lhs[0] / rhs[0], lhs[1] / rhs[1]);
}


template<class T>
class Vec3
{
public:

    /** Data type of vector components.*/
    typedef T scalar_type;

    /** Number of vector components. */
    enum { num_components = 3 };

    scalar_type _v[3];

    /** Constructor that sets all components of the vector to zero */
    Vec3() { _v[0] = 0.0f; _v[1] = 0.0f; _v[2] = 0.0f; }
    Vec3(scalar_type x, scalar_type y, scalar_type z) { _v[0] = x; _v[1] = y; _v[2] = z; }
    Vec3(scalar_type value) { _v[0] = value; _v[1] = value; _v[2] = value; }
    Vec3(const Vec2<T>& v2, scalar_type zz)
    {
        _v[0] = v2[0];
        _v[1] = v2[1];
        _v[2] = zz;
    }
    template<typename R>
    constexpr Vec3(R x, R y, R z) :
        _v{ static_cast<T>(x), static_cast<T>(y), static_cast<T>(z) } {}

    T* data() { return _v; }
    const T* data() const { return _v; }


    inline bool operator == (const Vec3& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2]; }

    inline bool operator != (const Vec3& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2]; }

    inline bool operator <  (const Vec3& v) const
    {
        if (_v[0] < v._v[0]) return true;
        else if (_v[0] > v._v[0]) return false;
        else if (_v[1] < v._v[1]) return true;
        else if (_v[1] > v._v[1]) return false;
        else return (_v[2] < v._v[2]);
    }

    inline scalar_type* ptr() { return _v; }
    inline const scalar_type* ptr() const { return _v; }

    inline void set(scalar_type x, scalar_type y, scalar_type z)
    {
        _v[0] = x; _v[1] = y; _v[2] = z;
    }

    inline void set(const Vec3& rhs)
    {
        _v[0] = rhs._v[0]; _v[1] = rhs._v[1]; _v[2] = rhs._v[2];
    }

    inline scalar_type& operator [] (int i) { return _v[i]; }
    inline scalar_type operator [] (int i) const { return _v[i]; }

    inline scalar_type& x() { return _v[0]; }
    inline scalar_type& y() { return _v[1]; }
    inline scalar_type& z() { return _v[2]; }

    inline scalar_type x() const { return _v[0]; }
    inline scalar_type y() const { return _v[1]; }
    inline scalar_type z() const { return _v[2]; }

    /** Returns true if all components have values that are not NaN. */
    //inline bool valid() const { return !isNaN(); }
    /** Returns true if at least one component has value NaN. */
    //inline bool isNaN() const { return osg::isNaN(_v[0]) || osg::isNaN(_v[1]) || osg::isNaN(_v[2]); }

    /** Dot product. */
    inline scalar_type operator * (const Vec3& rhs) const
    {
        return _v[0] * rhs._v[0] + _v[1] * rhs._v[1] + _v[2] * rhs._v[2];
    }

    /** Cross product. */
    inline const Vec3 operator ^ (const Vec3& rhs) const
    {
        return Vec3(_v[1] * rhs._v[2] - _v[2] * rhs._v[1],
            _v[2] * rhs._v[0] - _v[0] * rhs._v[2],
            _v[0] * rhs._v[1] - _v[1] * rhs._v[0]);
    }

    /** Multiply by scalar. */
    inline const Vec3 operator * (scalar_type rhs) const
    {
        return Vec3(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs);
    }

    /** Unary multiply by scalar. */
    inline Vec3& operator *= (scalar_type rhs)
    {
        _v[0] *= rhs;
        _v[1] *= rhs;
        _v[2] *= rhs;
        return *this;
    }

    /** Divide by scalar. */
    inline const Vec3 operator / (scalar_type rhs) const
    {
        return Vec3(_v[0] / rhs, _v[1] / rhs, _v[2] / rhs);
    }

    /** Unary divide by scalar. */
    inline Vec3& operator /= (scalar_type rhs)
    {
        _v[0] /= rhs;
        _v[1] /= rhs;
        _v[2] /= rhs;
        return *this;
    }

    /** Binary vector add. */
    inline const Vec3 operator + (const Vec3& rhs) const
    {
        return Vec3(_v[0] + rhs._v[0], _v[1] + rhs._v[1], _v[2] + rhs._v[2]);
    }

    /** Unary vector add. Slightly more efficient because no temporary
      * intermediate object.
    */
    inline Vec3& operator += (const Vec3& rhs)
    {
        _v[0] += rhs._v[0];
        _v[1] += rhs._v[1];
        _v[2] += rhs._v[2];
        return *this;
    }

    /** Binary vector subtract. */
    inline const Vec3 operator - (const Vec3& rhs) const
    {
        return Vec3(_v[0] - rhs._v[0], _v[1] - rhs._v[1], _v[2] - rhs._v[2]);
    }

    /** Unary vector subtract. */
    inline Vec3& operator -= (const Vec3& rhs)
    {
        _v[0] -= rhs._v[0];
        _v[1] -= rhs._v[1];
        _v[2] -= rhs._v[2];
        return *this;
    }

    /** Negation operator. Returns the negative of the Vec3f. */
    inline const Vec3 operator - () const
    {
        return Vec3(-_v[0], -_v[1], -_v[2]);
    }

    /** Length of the vector = sqrt( vec . vec ) */
    inline scalar_type length() const
    {
        return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);
    }

    /** Length squared of the vector = vec . vec */
    inline scalar_type length2() const
    {
        return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2];
    }

    /** Normalize the vector so that it has length unity.
      * Returns the previous length of the vector.
    */
    inline scalar_type normalize()
    {
        scalar_type norm = Vec3::length();
        if (norm > 0.0)
        {
            scalar_type inv = 1.0f / norm;
            _v[0] *= inv;
            _v[1] *= inv;
            _v[2] *= inv;
        }
        return(norm);
    }

};   
using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3ui = Vec3<uint32_t>;
using Vec3us = Vec3<uint16_t>;
using Vec3ub = Vec3<uint8_t>;

/** multiply by vector components. */
template<class T>
inline Vec3<T> componentMultiply(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    return Vec3<T>(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]);
}

/** divide rhs components by rhs vector components. */
template<typename T>
inline Vec3<T> componentDivide(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    return Vec3<T>(lhs[0] / rhs[0], lhs[1] / rhs[1], lhs[2] / rhs[2]);
}
template<typename T>
inline Vec3<T> normalize(const Vec3<T>& lhs)
{
    T norm = lhs.length();
    if (norm > 0.0)
    {
        T inv = 1.0f / norm;
        return Vec3<T>(lhs[0] *inv, lhs[1] * inv, lhs[2] * inv);
    }
    else
    {
        return Vec3<T>(lhs[0], lhs[1], lhs[2]);
    }
    
}
template<typename T>
constexpr Vec3<T> cross(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    return Vec3<T>(lhs[1] * rhs[2] - rhs[1] * lhs[2],
        lhs[2] * rhs[0] - rhs[2] * lhs[0],
        lhs[0] * rhs[1] - rhs[0] * lhs[1]);
}
template<typename T>
constexpr Vec3<T> operator^(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    return cross(lhs, rhs);
}
const Vec3f X_AXIS(1.0, 0.0, 0.0);
const Vec3f Y_AXIS(0.0, 1.0, 0.0);
const Vec3f Z_AXIS(0.0, 0.0, 1.0);

template<class T>
class Vec4
{
public:

    /** Data type of vector components.*/
    typedef T scalar_type;

    /** Number of vector components. */
    enum { num_components = 4 };

    /** Vec member variable. */
    scalar_type _v[4];

    // Methods are defined here so that they are implicitly inlined

    /** Constructor that sets all components of the vector to zero */
    Vec4() { _v[0] = 0.0f; _v[1] = 0.0f; _v[2] = 0.0f; _v[3] = 0.0f; }

    Vec4(scalar_type x, scalar_type y, scalar_type z, scalar_type w)
    {
        _v[0] = x;
        _v[1] = y;
        _v[2] = z;
        _v[3] = w;
    }
    template<typename R>
    Vec4(const std::vector<R>& v):_v{ static_cast<T>(v[0]), static_cast<T>(v[1]), static_cast<T>(v[2]), static_cast<T>(v[3]) } {}

    Vec4(const Vec3<T>& v3, scalar_type w)
    {
        _v[0] = v3[0];
        _v[1] = v3[1];
        _v[2] = v3[2];
        _v[3] = w;
    }

    template<typename R>
    constexpr explicit Vec4(const Vec4<R>& v) :
        _v{ static_cast<T>(v[0]), static_cast<T>(v[1]), static_cast<T>(v[2]), static_cast<T>(v[3]) } {}

    template<typename R>
    constexpr Vec4(R x, R y, R z, R w) :
        _v{ static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w) } {}
   
    //template<typename R>
    //inline bool operator = (const std::vector<R>& v) const {
    //    assert(v.size() == 4);
    //    _v[0] = v[0];
    //    _v[1] = v[1];
    //    _v[2] = v[2];
    //    _v[3] = v[3];
    //}

    T* data() { return _v; }
    const T* data() const { return _v; }
    inline bool operator == (const Vec4& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2] && _v[3] == v._v[3]; }

    inline bool operator != (const Vec4& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2] || _v[3] != v._v[3]; }

    inline bool operator <  (const Vec4& v) const
    {
        if (_v[0] < v._v[0]) return true;
        else if (_v[0] > v._v[0]) return false;
        else if (_v[1] < v._v[1]) return true;
        else if (_v[1] > v._v[1]) return false;
        else if (_v[2] < v._v[2]) return true;
        else if (_v[2] > v._v[2]) return false;
        else return (_v[3] < v._v[3]);
    }

    inline scalar_type* ptr() { return _v; }
    inline const scalar_type* ptr() const { return _v; }

    inline void set(scalar_type x, scalar_type y, scalar_type z, scalar_type w)
    {
        _v[0] = x; _v[1] = y; _v[2] = z; _v[3] = w;
    }

    inline scalar_type& operator [] (unsigned int i) { return _v[i]; }
    inline scalar_type  operator [] (unsigned int i) const { return _v[i]; }

    inline scalar_type& x() { return _v[0]; }
    inline scalar_type& y() { return _v[1]; }
    inline scalar_type& z() { return _v[2]; }
    inline scalar_type& w() { return _v[3]; }

    inline scalar_type x() const { return _v[0]; }
    inline scalar_type y() const { return _v[1]; }
    inline scalar_type z() const { return _v[2]; }
    inline scalar_type w() const { return _v[3]; }

    inline scalar_type& r() { return _v[0]; }
    inline scalar_type& g() { return _v[1]; }
    inline scalar_type& b() { return _v[2]; }
    inline scalar_type& a() { return _v[3]; }

    inline scalar_type r() const { return _v[0]; }
    inline scalar_type g() const { return _v[1]; }
    inline scalar_type b() const { return _v[2]; }
    inline scalar_type a() const { return _v[3]; }

    inline unsigned int asABGR() const
    {
        return (unsigned int)clampTo((_v[0] * 255.0f), 0.0f, 255.0f) << 24 |
            (unsigned int)clampTo((_v[1] * 255.0f), 0.0f, 255.0f) << 16 |
            (unsigned int)clampTo((_v[2] * 255.0f), 0.0f, 255.0f) << 8 |
            (unsigned int)clampTo((_v[3] * 255.0f), 0.0f, 255.0f);
    }

    inline unsigned int asRGBA() const
    {
        return (unsigned int)clampTo((_v[3] * 255.0f), 0.0f, 255.0f) << 24 |
            (unsigned int)clampTo((_v[2] * 255.0f), 0.0f, 255.0f) << 16 |
            (unsigned int)clampTo((_v[1] * 255.0f), 0.0f, 255.0f) << 8 |
            (unsigned int)clampTo((_v[0] * 255.0f), 0.0f, 255.0f);
    }

    /** Returns true if all components have values that are not NaN. */


    /** Dot product. */
    inline scalar_type operator * (const Vec4& rhs) const
    {
        return _v[0] * rhs._v[0] +
            _v[1] * rhs._v[1] +
            _v[2] * rhs._v[2] +
            _v[3] * rhs._v[3];
    }

    /** Multiply by scalar. */
    inline Vec4 operator * (scalar_type rhs) const
    {
        return Vec4(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs, _v[3] * rhs);
    }

    /** Unary multiply by scalar. */
    inline Vec4& operator *= (scalar_type rhs)
    {
        _v[0] *= rhs;
        _v[1] *= rhs;
        _v[2] *= rhs;
        _v[3] *= rhs;
        return *this;
    }

    /** Divide by scalar. */
    inline Vec4 operator / (scalar_type rhs) const
    {
        return Vec4(_v[0] / rhs, _v[1] / rhs, _v[2] / rhs, _v[3] / rhs);
    }

    /** Unary divide by scalar. */
    inline Vec4& operator /= (scalar_type rhs)
    {
        _v[0] /= rhs;
        _v[1] /= rhs;
        _v[2] /= rhs;
        _v[3] /= rhs;
        return *this;
    }

    /** Binary vector add. */
    inline Vec4 operator + (const Vec4& rhs) const
    {
        return Vec4(_v[0] + rhs._v[0], _v[1] + rhs._v[1],
            _v[2] + rhs._v[2], _v[3] + rhs._v[3]);
    }

    /** Unary vector add. Slightly more efficient because no temporary
      * intermediate object.
    */
    inline Vec4& operator += (const Vec4& rhs)
    {
        _v[0] += rhs._v[0];
        _v[1] += rhs._v[1];
        _v[2] += rhs._v[2];
        _v[3] += rhs._v[3];
        return *this;
    }

    /** Binary vector subtract. */
    inline Vec4 operator - (const Vec4& rhs) const
    {
        return Vec4(_v[0] - rhs._v[0], _v[1] - rhs._v[1],
            _v[2] - rhs._v[2], _v[3] - rhs._v[3]);
    }

    /** Unary vector subtract. */
    inline Vec4& operator -= (const Vec4& rhs)
    {
        _v[0] -= rhs._v[0];
        _v[1] -= rhs._v[1];
        _v[2] -= rhs._v[2];
        _v[3] -= rhs._v[3];
        return *this;
    }

    /** Negation operator. Returns the negative of the Vec4f. */
    inline const Vec4 operator - () const
    {
        return Vec4(-_v[0], -_v[1], -_v[2], -_v[3]);
    }

    /** Length of the vector = sqrt( vec . vec ) */
    inline scalar_type length() const
    {
        return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3]);
    }

    /** Length squared of the vector = vec . vec */
    inline scalar_type length2() const
    {
        return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3];
    }

    /** Normalize the vector so that it has length unity.
      * Returns the previous length of the vector.
    */
    inline scalar_type normalize()
    {
        scalar_type norm = Vec4::length();
        if (norm > 0.0f)
        {
            scalar_type inv = 1.0f / norm;
            _v[0] *= inv;
            _v[1] *= inv;
            _v[2] *= inv;
            _v[3] *= inv;
        }
        return(norm);
    }

};    // end of class Vec4f

using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;
