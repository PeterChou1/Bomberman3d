#pragma once

//---------------------------------------------------------------------------------
// MathUtil.h
// Includes core valath utilities
//---------------------------------------------------------------------------------
#include <sstream>

struct Vec3d
{
	double X, Y, Z;
};

inline Vec3d Cross(const Vec3d& a, const Vec3d& b)
{
	return {
			a.Y * b.Z - a.Z * b.Y,
			a.Z * b.X - a.X * b.Z,
			a.X * b.Y - a.Y * b.X
	};
}

inline double Dot(const Vec3d& a,const Vec3d& b)
{
	return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}


inline double Mag(const Vec3d& a)
{
	return sqrt(pow(a.X, 2) + pow(a.Y, 2) + pow(a.Z, 2));
}

inline void Normalize(Vec3d& a)
{
	const double n = Mag(a);
	a.X /= n;
	a.Y /= n;
	a.Z /= n;
}


inline Vec3d operator*(const double& a, const Vec3d& b)
{
	return { a * b.X, a * b.Y, a * b.Z };
}

inline Vec3d operator*(const Vec3d& a, const double& b)
{
	return b * a;
}

inline Vec3d operator+(const Vec3d& a, const Vec3d& b)
{ 
	return { a.X + b.X, a.Y + b.Y, a.Z + b.Z };
}

inline Vec3d operator-(const Vec3d& a, const Vec3d& b)
{
	return { a.X - b.X, a.Y - b.Y, a.Z - b.Z };
}

inline Vec3d operator/(const Vec3d& a, const double& b)
{
	return { a.X / b, a.Y / b, a.Z / b };
}

