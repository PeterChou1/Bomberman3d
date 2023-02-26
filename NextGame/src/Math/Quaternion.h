#pragma once
#include "Matrix.h"
#include "Vec3d.h"


struct Quat
{
	double s;
	Vec3d v;
};

inline void Quat2Matrix(const Quat& q, Mat4X4& m) {

    double x = q.v.X;
    double y = q.v.Y;
    double z = q.v.Z;
    double w = q.s;

    double xx = x * x;
    double xy = x * y;
    double xz = x * z;
    double xw = x * w;

    double yy = y * y;
    double yz = y * z;
    double yw = y * w;

    double zz = z * z;
    double zw = z * w;

    m[0][0] = 1 - 2 * (yy + zz);
    m[0][1] = 2 * (xy - zw);
    m[0][2] = 2 * (xz + yw);
    m[0][3] = 0;

    m[1][0] = 2 * (xy + zw);
    m[1][1] = 1 - 2 * (xx + zz);
    m[1][2] = 2 * (yz - xw);
    m[1][3] = 0;

    m[2][0] = 2 * (xz - yw);
    m[2][1] = 2 * (yz + xw);
    m[2][2] = 1 - 2 * (xx + yy);
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

inline void Matrix2Quat(const Mat4X4& m, Quat& q) {
    double trace = m[0][0] + m[1][1] + m[2][2];
    double k;

    if (trace > 0.0) {
        k = 0.5 / std::sqrt(trace + 1.0);
        q.s = 0.25 / k;
        q.v.X = k * (m[1][2] - m[2][1]);
        q.v.Y = k * (m[2][0] - m[0][2]);
        q.v.Z = k * (m[0][1] - m[1][0]);
    }
    else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2])) {
        k = 0.5 / std::sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]);
        q.s = 0.25 / k;
        q.v.X = k * (m[0][1] + m[1][0]);
        q.v.Y = k * (m[0][2] + m[2][0]);
        q.v.Z = k * (m[2][1] - m[1][2]);
    }
    else if (m[1][1] > m[2][2]) {
        k = 0.5 / std::sqrt(1.0 + m[1][1] - m[0][0] - m[2][2]);
        q.s = 0.25 / k;
        q.v.X = k * (m[0][1] + m[1][0]);
        q.v.Y = k * (m[2][1] + m[1][2]);
        q.v.Z = k * (m[2][0] - m[0][2]);
    }
    else {
        k = 0.5 / std::sqrt(1.0 + m[2][2] - m[0][0] - m[1][1]);
        q.s = 0.25 / k;
        q.v.X = k * (m[2][0] + m[0][2]);
        q.v.Y = k * (m[2][1] + m[1][2]);
        q.v.Z = k * (m[1][0] - m[0][1]);
    }
}

inline Quat Euler2Quat(const Vec3d& r)
{
	Quat q{};
	const double yaw = r.X;
	const double pitch = r.Y;
	const double roll = r.Z;
	q.s = cos(roll / 2) * cos(pitch / 2) * cos(yaw / 2) + sin(roll / 2) * sin(pitch / 2) * sin(yaw / 2);
	q.v.Z = sin(roll / 2) * cos(pitch / 2) * cos(yaw / 2) - cos(roll / 2) * sin(pitch / 2) * sin(yaw / 2);
	q.v.Y = cos(roll / 2) * sin(pitch / 2) * cos(yaw / 2) + sin(roll / 2) * cos(pitch / 2) * sin(yaw / 2);
	q.v.X = cos(roll / 2) * cos(pitch / 2) * sin(yaw / 2) - sin(roll / 2) * sin(pitch / 2) * cos(yaw / 2);
	return q;
}

inline Vec3d Quat2Euler(const Quat& q)
{
	double t0 = 2.0 * (q.s * q.v.X + q.v.Y * q.v.Z);
	double t1 = 1.0 - 2.0 * (q.v.X * q.v.X + q.v.Y * q.v.Y);
	double roll = atan2(t0, t1);
	double t2 = 2.0 * (q.s * q.v.Y - q.v.Z * q.v.X);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	double pitch = asin(t2);
	double t3 = 2.0 * (q.s * q.v.Z + q.v.X * q.v.Y);
	double t4 = 1.0 - 2.0 * (q.v.Y * q.v.Y + q.v.Z * q.v.Z);
	double yaw = atan2(t3, t4);
	return { yaw, pitch, roll };
}

inline Quat operator*(const Quat& a, const Quat& b)
{
	Quat product{};
	product.s = a.s * b.s - Dot(a.v, b.v);
	product.v = a.s * b.v + b.s * a.v + Cross(a.v, b.v);
	return product;
}

inline double Mag(const Quat& q) {
    return std::sqrt(q.s * q.s + q.v.X * q.v.X + q.v.Y * q.v.Y + q.v.Z * q.v.Z);
}

inline void Normalize(Quat& q)
{
    double norm = Mag(q);
    q.s = q.s / norm;
	q.v.X = q.v.X / norm;
    q.v.Y = q.v.Y / norm;
    q.v.Z = q.v.Z / norm;
}


inline Quat Conjugate(const Quat& q) {
    return { q.s, {-q.v.X, -q.v.Y, -q.v.Z} };
}

inline Vec3d RotateVector(const Quat& q, const Vec3d& v) {
    // Convert the vector to a quaternion
    Quat p = { 0, {v.X, v.Y, v.Z} };
    // Calculate the quaternion product q * p * q^-1
    Quat result = q * p * Conjugate(q);
    // Return the vector part of the result quaternion
    return result.v;
}

