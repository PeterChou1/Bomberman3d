#pragma once

//---------------------------------------------------------------------------------
// MathUtil.h
// Includes core valath utilities
//---------------------------------------------------------------------------------
#include <sstream>

struct Vec3d
{
	double x, y, z;

	// dot
	double dot(const Vec3d& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	// cross product
	Vec3d cross(const Vec3d& other) const
	{
		return {
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		};
	}

	double norm() const
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	void normalize()
	{
		const double n = norm();
		x /= n;
		y /= n;
		z /= n;
	}
};



inline Vec3d operator*(const double& a, const Vec3d& b)
{
	return { a * b.x, a * b.y, a * b.z };
}

inline Vec3d operator*(const Vec3d& a, const double& b)
{
	return b * a;
}

inline Vec3d operator+(const Vec3d& a, const Vec3d& b)
{ 
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline Vec3d operator-(const Vec3d& a, const Vec3d& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline Vec3d operator/(const Vec3d& a, const double& b)
{
	return { a.x / b, a.y / b, a.z / b };
}

