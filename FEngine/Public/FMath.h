#pragma once

#include <cfloat>
#include <cmath>
#include<cstdlib>
#include<cstdint>
using namespace std;





	
#ifdef PI
#undef PI
#undef PI_2
#undef PI_4
#endif

const double PI = 3.14159265358979323846;
const double PI_2 = 1.57079632679489661923;
const double PI_4 = 0.78539816339744830962;
const double LN_2 = 0.69314718055994530942;
const double INVLN_2 = 1.0 / LN_2;

const float PIf = 3.14159265358979323846f;
const float PI_2f = 1.57079632679489661923f;
const float PI_4f = 0.78539816339744830962f;
const float LN_2f = 0.69314718055994530942f;
const float INVLN_2f = 1.0f / LN_2f;


template<typename T>
inline T default_value() { return T(); }

template<> inline float default_value<float>() { return 0.0f; }
template<> inline double default_value<double>() { return 0.0; }
template<> inline char default_value<char>() { return 0; }
template<> inline unsigned char default_value<unsigned char>() { return 0; }
template<> inline short default_value<short>() { return 0; }
template<> inline unsigned short default_value<unsigned short>() { return 0; }
template<> inline int default_value<int>() { return 0; }
template<> inline unsigned int default_value<unsigned int>() { return 0; }


template<typename T>
inline T absolute(T v) { return v < (T)0 ? -v : v; }

/** return true if float lhs and rhs are equivalent,
	* meaning that the difference between them is less than an epsilon value
	* which defaults to 1e-6.
*/
inline bool equivalent(float lhs, float rhs, float epsilon = 1e-6)
{
	float delta = rhs - lhs; return delta < 0.0f ? delta >= -epsilon : delta <= epsilon;
}

/** return true if double lhs and rhs are equivalent,
	* meaning that the difference between them is less than an epsilon value
	* which defaults to 1e-6.
*/
inline bool equivalent(double lhs, double rhs, double epsilon = 1e-6)
{
	double delta = rhs - lhs; return delta < 0.0 ? delta >= -epsilon : delta <= epsilon;
}

/** return the minimum of two values, equivalent to std::min.
	* std::min not used because of STL implementation under IRIX not containing
	* std::min.
*/
template<typename T>
inline T minimum(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }

/** return the maximum of two values, equivalent to std::max.
	* std::max not used because of STL implementation under IRIX not containing
	* std::max.
*/
template<typename T>
inline T maximum(T lhs, T rhs) { return lhs > rhs ? lhs : rhs; }

template<typename T>
inline T clampTo(T v, T minimum, T maximum) { return v<minimum ? minimum : v>maximum ? maximum : v; }

template<typename T>
inline T clampAbove(T v, T minimum) { return v < minimum ? minimum : v; }

template<typename T>
inline T clampBelow(T v, T maximum) { return v > maximum ? maximum : v; }

template<typename T>
inline T clampBetween(T v, T minimum, T maximum)
{
	return clampBelow(clampAbove(v, minimum), maximum);
}

template<typename T>
inline T sign(T v) { return v < (T)0 ? (T)-1 : (T)1; }

template<typename T>
inline T signOrZero(T v) { return v < (T)0 ? (T)-1 : (v > (T)0 ? (T)1 : 0); }

template<typename T>
inline T square(T v) { return v * v; }

template<typename T>
inline T signedSquare(T v) { return v < (T)0 ? -v * v : v * v;; }

inline float inDegrees(float angle) { return angle * (float)PI / 180.0f; }
inline double inDegrees(double angle) { return angle * PI / 180.0; }

template<typename T>
inline T inRadians(T angle) { return angle; }

inline float DegreesToRadians(float angle) { return angle * (float)PI / 180.0f; }
inline double DegreesToRadians(double angle) { return angle * PI / 180.0; }

inline float RadiansToDegrees(float rad) { return rad * 180.0f / (float)PI; }
inline double RadiansToDegrees(double rad) { return rad * 180.0 / PI; }

//inline float round(float v) { return v >= 0.0f ? floorf(v + 0.5f) : ceilf(v - 0.5f); }
//inline double round(double v) { return v >= 0.0 ? floor(v + 0.5) : ceil(v - 0.5); }
