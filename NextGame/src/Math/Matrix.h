#pragma once

#include "Vec3d.h"
#include <array>

struct Mat3X3
{
	std::array<std::array<double, 3>, 3> Val;

	Mat3X3 Inverse() const;

	std::array<double, 3> operator[](const int i) const { return Val[i]; }
	std::array<double, 3>& operator[](const int i) { return Val[i]; }

	static Mat3X3 Identity()
	{
		Mat3X3 identity = {};
		for (int i = 0; i < 3; i++)
			identity[i][i] = 1;
		return identity;
	}
};
inline void normalize(Mat3X3& a)
{
	double col0Mag = sqrt(pow(a[0][0], 2) + pow(a[1][0], 2) + pow(a[2][0], 2));
	double col1Mag = sqrt(pow(a[0][1], 2) + pow(a[1][1], 2) + pow(a[2][1], 2));
	double col2Mag = sqrt(pow(a[0][2], 2) + pow(a[1][2], 2) + pow(a[2][2], 2));

	a[0][0] /= col0Mag;
	a[0][1] /= col0Mag;
	a[0][2] /= col0Mag;

	a[1][0] /= col1Mag;
	a[1][1] /= col1Mag;
	a[1][2] /= col1Mag;

	a[2][0] /= col2Mag;
	a[2][1] /= col2Mag;
	a[2][2] /= col2Mag;
}

inline Mat3X3 operator+(const Mat3X3& a, const Mat3X3& b)
{
	Mat3X3 m{};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m[i][j] = a[i][j] + b[i][j];
	return m;
}

inline Mat3X3 Transpose(const Mat3X3& a)
{
	Mat3X3 m{};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m[i][j] = a[i][j];

	return m;
}

inline Mat3X3 operator*(const Mat3X3& a, const Mat3X3& b)
{
	Mat3X3 m{};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			m[i][j] = a[i][0] * b[0][j];
			m[i][j] += a[i][1] * b[1][j];
			m[i][j] += a[i][2] * b[2][j];
		}
	return m;
}

inline Mat3X3 operator*(const Mat3X3& a, const double b)
{
	Mat3X3 m{};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m[i][j] = a[i][j] * b;
	return m;
}

inline Mat3X3 operator*(const double a, const Mat3X3& b)
{
	return b * a;
}

inline Vec3d operator*(const Mat3X3& a, const Vec3d& b)
{
	Vec3d c{};
	c.X = a[0][0] * b.X + a[0][1] * b.Y + a[0][2] * b.Z;
	c.Y = a[1][0] * b.X + a[1][1] * b.Y + a[1][2] * b.Z;
	c.Z = a[2][0] * b.X + a[2][1] * b.Y + a[2][2] * b.Z;
	return c;
}

inline bool operator==(const Mat3X3& a, const Mat3X3& b)
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

struct Mat4X4
{
	std::array<std::array<double, 4>, 4> Val;

	Mat4X4 Inverse() const;

	std::array<double, 4> operator[](const int i) const { return Val[i]; }
	std::array<double, 4>& operator[](const int i) { return Val[i]; }

	static Mat4X4 Identity()
	{
		Mat4X4 identity = {};
		for (int i = 0; i < 4; i++)
			identity[i][i] = 1;
		return identity;
	}
};


inline Mat4X4 Transpose(const Mat4X4& a)
{
	Mat4X4 m{};
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = a[i][j];

	return m;
}

inline Mat4X4 operator*(const Mat4X4& a, const double b)
{
	Mat4X4 m{};
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = a[i][j] * b;
	return m;
}

inline Mat4X4 operator*(const double a, const Mat4X4& b)
{
	return b * a;
}

inline Mat4X4 operator*(const Mat4X4& a, const Mat4X4& b)
{
	Mat4X4 m{};
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = a[i][0] * b[0][j];
			m[i][j] += a[i][1] * b[1][j];
			m[i][j] += a[i][2] * b[2][j];
			m[i][j] += a[i][3] * b[3][j];
		}
	return m;
}

inline Vec3d operator*(const Mat4X4& a, const Vec3d& b)
{
	Vec3d c{};
	c.X = a[0][0] * b.X + a[0][1] * b.Y + a[0][2] * b.Z + a[0][3];
	c.Y = a[1][0] * b.X + a[1][1] * b.Y + a[1][2] * b.Z + a[1][3];
	c.Z = a[2][0] * b.X + a[2][1] * b.Y + a[2][2] * b.Z + a[2][3];
	const double w = a[3][0] * b.X + a[3][1] * b.Y + a[3][2] * b.Z + a[3][3];

	if (w != 1)
	{
		c.X /= w;
		c.Y /= w;
		c.Z /= w;
	}
	return c;
}

inline bool operator==(const Mat4X4& a, const Mat4X4& b)
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
