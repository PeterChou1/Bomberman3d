#include "stdafx.h"
#include "Matrix.h"


// implements inverse algorithm brute force (3x3) and (4x4) : https://stackoverflow.com/a/18504573

Mat3x3 Mat3x3::inverse() {
	Mat3x3 inv = {};

	// computes the inverse of a matrix m
	float det = val[0][0] * (val[1][1] * val[2][2] - val[2][1] * val[1][2]) - val[0][1] * (val[1][0] * val[2][2] - val[1][2] * val[2][0]) + val[0][2] * (val[1][0] * val[2][1] - val[1][1] * val[2][0]);

	// inverse does not exist return empty matrix
	if (std::abs(det) <= std::numeric_limits<double>::epsilon()) return inv;

	float invdet = 1 / det;

	inv[0][0] = (val[1][1] * val[2][2] - val[2][1] * val[1][2]) * invdet;
	inv[0][1] = (val[0][2] * val[2][1] - val[0][1] * val[2][2]) * invdet;
	inv[0][2] = (val[0][1] * val[1][2] - val[0][2] * val[1][1]) * invdet;
	inv[1][0] = (val[1][2] * val[2][0] - val[1][0] * val[2][2]) * invdet;
	inv[1][1] = (val[0][0] * val[2][2] - val[0][2] * val[2][0]) * invdet;
	inv[1][2] = (val[1][0] * val[0][2] - val[0][0] * val[1][2]) * invdet;
	inv[2][0] = (val[1][0] * val[2][1] - val[2][0] * val[1][1]) * invdet;
	inv[2][1] = (val[2][0] * val[0][1] - val[0][0] * val[2][1]) * invdet;
	inv[2][2] = (val[0][0] * val[1][1] - val[1][0] * val[0][1]) * invdet;

	return inv;
}

Mat4x4 Mat4x4::inverse() {
	Mat4x4 inv = {};

	auto A2323 = val[2][2] * val[3][3] - val[2][3] * val[3][2];
	auto A1323 = val[2][1] * val[3][3] - val[2][3] * val[3][1];
	auto A1223 = val[2][1] * val[3][2] - val[2][2] * val[3][1];
	auto A0323 = val[2][0] * val[3][3] - val[2][3] * val[3][0];
	auto A0223 = val[2][0] * val[3][2] - val[2][2] * val[3][0];
	auto A0123 = val[2][0] * val[3][1] - val[2][1] * val[3][0];
	auto A2313 = val[1][2] * val[3][3] - val[1][3] * val[3][2];
	auto A1313 = val[1][1] * val[3][3] - val[1][3] * val[3][1];
	auto A1213 = val[1][1] * val[3][2] - val[1][2] * val[3][1];
	auto A2312 = val[1][2] * val[2][3] - val[1][3] * val[2][2];
	auto A1312 = val[1][1] * val[2][3] - val[1][3] * val[2][1];
	auto A1212 = val[1][1] * val[2][2] - val[1][2] * val[2][1];
	auto A0313 = val[1][0] * val[3][3] - val[1][3] * val[3][0];
	auto A0213 = val[1][0] * val[3][2] - val[1][2] * val[3][0];
	auto A0312 = val[1][0] * val[2][3] - val[1][3] * val[2][0];
	auto A0212 = val[1][0] * val[2][2] - val[1][2] * val[2][0];
	auto A0113 = val[1][0] * val[3][1] - val[1][1] * val[3][0];
	auto A0112 = val[1][0] * val[2][1] - val[1][1] * val[2][0];

	auto det = val[0][0] * (val[1][1] * A2323 - val[1][2] * A1323 + val[1][3] * A1223)
		- val[0][1] * (val[1][0] * A2323 - val[1][2] * A0323 + val[1][3] * A0223)
		+ val[0][2] * (val[1][0] * A1323 - val[1][1] * A0323 + val[1][3] * A0123)
		- val[0][3] * (val[1][0] * A1223 - val[1][1] * A0223 + val[1][2] * A0123);

	// inverse does not exist return empty matrix
	if (std::abs(det) <= std::numeric_limits<double>::epsilon()) return inv;

	det = 1 / det;
	inv[0][0] = det * (val[1][1] * A2323 - val[1][2] * A1323 + val[1][3] * A1223);
	inv[0][1] = det * -(val[0][1] * A2323 - val[0][2] * A1323 + val[0][3] * A1223);
	inv[0][2] = det * (val[0][1] * A2313 - val[0][2] * A1313 + val[0][3] * A1213);
	inv[0][3] = det * -(val[0][1] * A2312 - val[0][2] * A1312 + val[0][3] * A1212);
	inv[1][0] = det * -(val[1][0] * A2323 - val[1][2] * A0323 + val[1][3] * A0223);
	inv[1][1] = det * (val[0][0] * A2323 - val[0][2] * A0323 + val[0][3] * A0223);
	inv[1][2] = det * -(val[0][0] * A2313 - val[0][2] * A0313 + val[0][3] * A0213);
	inv[1][3] = det * (val[0][0] * A2312 - val[0][2] * A0312 + val[0][3] * A0212);
	inv[2][0] = det * (val[1][0] * A1323 - val[1][1] * A0323 + val[1][3] * A0123);
	inv[2][1] = det * -(val[0][0] * A1323 - val[0][1] * A0323 + val[0][3] * A0123);
	inv[2][2] = det * (val[0][0] * A1313 - val[0][1] * A0313 + val[0][3] * A0113);
	inv[2][3] = det * -(val[0][0] * A1312 - val[0][1] * A0312 + val[0][3] * A0112);
	inv[3][0] = det * -(val[1][0] * A1223 - val[1][1] * A0223 + val[1][2] * A0123);
	inv[3][1] = det * (val[0][0] * A1223 - val[0][1] * A0223 + val[0][2] * A0123);
	inv[3][2] = det * -(val[0][0] * A1213 - val[0][1] * A0213 + val[0][2] * A0113);
	inv[3][3] = det * (val[0][0] * A1212 - val[0][1] * A0212 + val[0][2] * A0112);

	return inv;

}