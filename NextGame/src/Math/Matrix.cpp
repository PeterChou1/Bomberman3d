#include "stdafx.h"
#include "Matrix.h"


// implements inverse algorithm brute force (3x3) and (4x4) : https://stackoverflow.com/a/18504573

Mat3X3 Mat3X3::Inverse() const
{
	Mat3X3 inv = {};

	// computes the inverse of a matrix m
	const float det = Val[0][0] * (Val[1][1] * Val[2][2] - Val[2][1] * Val[1][2]) - Val[0][1] * (Val[1][0] * Val[2][2] -
		Val[1][2] * Val[2][0]) + Val[0][2] * (Val[1][0] * Val[2][1] - Val[1][1] * Val[2][0]);

	// inverse does not exist return empty matrix
	if (std::abs(det) <= std::numeric_limits<double>::epsilon()) return inv;

	float invdet = 1 / det;

	inv[0][0] = (Val[1][1] * Val[2][2] - Val[2][1] * Val[1][2]) * invdet;
	inv[0][1] = (Val[0][2] * Val[2][1] - Val[0][1] * Val[2][2]) * invdet;
	inv[0][2] = (Val[0][1] * Val[1][2] - Val[0][2] * Val[1][1]) * invdet;
	inv[1][0] = (Val[1][2] * Val[2][0] - Val[1][0] * Val[2][2]) * invdet;
	inv[1][1] = (Val[0][0] * Val[2][2] - Val[0][2] * Val[2][0]) * invdet;
	inv[1][2] = (Val[1][0] * Val[0][2] - Val[0][0] * Val[1][2]) * invdet;
	inv[2][0] = (Val[1][0] * Val[2][1] - Val[2][0] * Val[1][1]) * invdet;
	inv[2][1] = (Val[2][0] * Val[0][1] - Val[0][0] * Val[2][1]) * invdet;
	inv[2][2] = (Val[0][0] * Val[1][1] - Val[1][0] * Val[0][1]) * invdet;

	return inv;
}

Mat4X4 Mat4X4::Inverse() const
{
	Mat4X4 inv = {};

	const auto a2323 = Val[2][2] * Val[3][3] - Val[2][3] * Val[3][2];
	const auto a1323 = Val[2][1] * Val[3][3] - Val[2][3] * Val[3][1];
	const auto a1223 = Val[2][1] * Val[3][2] - Val[2][2] * Val[3][1];
	const auto a0323 = Val[2][0] * Val[3][3] - Val[2][3] * Val[3][0];
	const auto a0223 = Val[2][0] * Val[3][2] - Val[2][2] * Val[3][0];
	const auto a0123 = Val[2][0] * Val[3][1] - Val[2][1] * Val[3][0];
	const auto a2313 = Val[1][2] * Val[3][3] - Val[1][3] * Val[3][2];
	const auto a1313 = Val[1][1] * Val[3][3] - Val[1][3] * Val[3][1];
	const auto a1213 = Val[1][1] * Val[3][2] - Val[1][2] * Val[3][1];
	const auto a2312 = Val[1][2] * Val[2][3] - Val[1][3] * Val[2][2];
	const auto a1312 = Val[1][1] * Val[2][3] - Val[1][3] * Val[2][1];
	const auto a1212 = Val[1][1] * Val[2][2] - Val[1][2] * Val[2][1];
	const auto a0313 = Val[1][0] * Val[3][3] - Val[1][3] * Val[3][0];
	const auto a0213 = Val[1][0] * Val[3][2] - Val[1][2] * Val[3][0];
	const auto a0312 = Val[1][0] * Val[2][3] - Val[1][3] * Val[2][0];
	const auto a0212 = Val[1][0] * Val[2][2] - Val[1][2] * Val[2][0];
	const auto a0113 = Val[1][0] * Val[3][1] - Val[1][1] * Val[3][0];
	const auto a0112 = Val[1][0] * Val[2][1] - Val[1][1] * Val[2][0];

	auto det = Val[0][0] * (Val[1][1] * a2323 - Val[1][2] * a1323 + Val[1][3] * a1223)
		- Val[0][1] * (Val[1][0] * a2323 - Val[1][2] * a0323 + Val[1][3] * a0223)
		+ Val[0][2] * (Val[1][0] * a1323 - Val[1][1] * a0323 + Val[1][3] * a0123)
		- Val[0][3] * (Val[1][0] * a1223 - Val[1][1] * a0223 + Val[1][2] * a0123);

	// inverse does not exist return empty matrix
	if (std::abs(det) <= std::numeric_limits<double>::epsilon()) return inv;

	det = 1 / det;
	inv[0][0] = det * (Val[1][1] * a2323 - Val[1][2] * a1323 + Val[1][3] * a1223);
	inv[0][1] = det * -(Val[0][1] * a2323 - Val[0][2] * a1323 + Val[0][3] * a1223);
	inv[0][2] = det * (Val[0][1] * a2313 - Val[0][2] * a1313 + Val[0][3] * a1213);
	inv[0][3] = det * -(Val[0][1] * a2312 - Val[0][2] * a1312 + Val[0][3] * a1212);
	inv[1][0] = det * -(Val[1][0] * a2323 - Val[1][2] * a0323 + Val[1][3] * a0223);
	inv[1][1] = det * (Val[0][0] * a2323 - Val[0][2] * a0323 + Val[0][3] * a0223);
	inv[1][2] = det * -(Val[0][0] * a2313 - Val[0][2] * a0313 + Val[0][3] * a0213);
	inv[1][3] = det * (Val[0][0] * a2312 - Val[0][2] * a0312 + Val[0][3] * a0212);
	inv[2][0] = det * (Val[1][0] * a1323 - Val[1][1] * a0323 + Val[1][3] * a0123);
	inv[2][1] = det * -(Val[0][0] * a1323 - Val[0][1] * a0323 + Val[0][3] * a0123);
	inv[2][2] = det * (Val[0][0] * a1313 - Val[0][1] * a0313 + Val[0][3] * a0113);
	inv[2][3] = det * -(Val[0][0] * a1312 - Val[0][1] * a0312 + Val[0][3] * a0112);
	inv[3][0] = det * -(Val[1][0] * a1223 - Val[1][1] * a0223 + Val[1][2] * a0123);
	inv[3][1] = det * (Val[0][0] * a1223 - Val[0][1] * a0223 + Val[0][2] * a0123);
	inv[3][2] = det * -(Val[0][0] * a1213 - Val[0][1] * a0213 + Val[0][2] * a0113);
	inv[3][3] = det * (Val[0][0] * a1212 - Val[0][1] * a0212 + Val[0][2] * a0112);

	return inv;
}
