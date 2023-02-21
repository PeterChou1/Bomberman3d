#pragma once

#include "Vec3d.h"
#include <array>

struct Mat3x3
{
	std::array<std::array<double, 3>, 3>  val;

	Mat3x3 inverse();

	std::array<double, 3> operator[](const int i) const { return val[i]; }
	std::array<double, 3>& operator[](const int i) { return val[i]; }

	void transpose()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				val[i][j] = (*this)[j][i];
	}

	static Mat3x3 Identity()
	{
		Mat3x3 identity = {};
		for (int i = 0; i < 3; i++)
			identity[i][i] = 1;
		return identity;
	}
};

inline Mat3x3 operator*(const Mat3x3& a, const Mat3x3& b) {
	Mat3x3 m = {};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			m[i][j] = a[i][0] * b[0][j];
			m[i][j] += a[i][1] * b[1][j];
			m[i][j] += a[i][2] * b[2][j];
		}
	return m;
}

inline Vec3d operator*(const Mat3x3& a, const Vec3d& b)
{
	Vec3d c;
	c.x = a[0][0] * b.x + a[0][1] * b.y + a[0][2];
	c.y = a[1][0] * b.x + a[1][1] * b.y + a[1][2];
	c.z = a[2][0] * b.x + a[2][1] * b.y + a[2][2];
	return c;
}

inline bool operator==(const Mat3x3& a, const Mat3x3& b)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (abs(a[i][j] - b[i][j]) > std::numeric_limits<double>::epsilon())
			{
				return false;
			}
		}
	}
	return true;
}

struct Mat4x4
{
	std::array<std::array<double, 4>, 4>  val;

	Mat4x4 inverse();

	std::array<double, 4> operator[](const int i) const { return val[i]; }
	std::array<double, 4>& operator[](const int i) { return val[i]; }


	void transpose()
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				val[i][j] = (*this)[j][i];
	}

	static Mat4x4 Identity()
	{
		Mat4x4 identity = {};
		for (int i = 0; i < 4; i++)
			identity[i][i] = 1;
		return identity;
	}
};

inline Mat4x4 transpose(const Mat4x4& a)
{
	Mat4x4 m = {};
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = a[i][j];
}

inline Mat4x4 operator*(const Mat4x4& a, const double& b)
{
	Mat4x4 m = {};
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = a[i][j] * b;
	return m;
}

inline Mat4x4 operator*(const Mat4x4& a, const Mat4x4& b) {
	Mat4x4 m = {};
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			m[i][j] = a[i][0] * b[0][j];
			m[i][j] += a[i][1] * b[1][j];
			m[i][j] += a[i][2] * b[2][j];
			m[i][j] += a[i][3] * b[3][j];
		}
	return m;
}

inline Vec3d operator*(const Mat4x4& a, const Vec3d& b)
{
	Vec3d c;
	c.x = a[0][0] * b.x + a[0][1] * b.y + a[0][2] * b.z + a[0][3];
	c.y = a[1][0] * b.x + a[1][1] * b.y + a[1][2] * b.z + a[1][3];
	c.z = a[2][0] * b.x + a[2][1] * b.y + a[2][2] * b.z + a[2][3];
	const double w = a[3][0] * b.x + a[3][1] * b.y + a[3][2] * b.z + a[3][3];

	if (w != 1)
	{
		c.x /= w;
		c.y /= w;
		c.z /= w;
	}
	return c;
}

inline bool operator==(const Mat4x4& a, const Mat4x4& b)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (abs(a[i][j] - b[i][j]) > std::numeric_limits<double>::epsilon())
			{
				return false;
			}
		}
	}
	return true;
}

