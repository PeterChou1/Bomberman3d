#pragma once
#include <cassert>
#include <iostream>

#include "app/AppSettings.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vec3d.h"

struct Transform
{
	// x, y, z coordinates
	Vec3d Position;
	// quaternion representing rotations
	Quat Rotation;
	// scaling factor
	Vec3d Scaling;

	Mat4X4 Local2World;

	Mat4X4 World2Local;
};


inline Mat3X3 GetRotationMatrix(const Transform& t)
{
	Mat3X3 rotation{};
	rotation[0] = {t.Local2World[0][0], t.Local2World[0][1], t.Local2World[0][2]};
	rotation[1] = {t.Local2World[1][0], t.Local2World[1][1], t.Local2World[1][2]};
	rotation[2] = {t.Local2World[2][0], t.Local2World[2][1], t.Local2World[2][2]};
	return rotation;
}


/**
 * \brief Given a 3 x 3 Affine Matrix R decompose it to find its euler angle
 *		  algorithmn described here: http://eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf
 */
inline void GetEulerAngles(Mat4X4& rotation, Vec3d& eulerAngles)
{
	if (rotation[2][0] != 1 || rotation[2][0] != -1)
	{
		eulerAngles.Y = -asin(rotation[2][0]);
		eulerAngles.X = atan2(rotation[2][1] / cos(eulerAngles.Y), rotation[2][2] / cos(eulerAngles.Y));
		eulerAngles.Z = atan2(rotation[1][0] / cos(eulerAngles.Y), rotation[0][0] / cos(eulerAngles.Y));
	}
	else
	{
		// Gimbal Lock Scenerio
		std::cout << "Gimbal lock\n";
		eulerAngles.Z = 0;
		if (rotation[2][0] == -1)
		{
			eulerAngles.Y = PI / 2;
			eulerAngles.X = atan2(rotation[0][1], rotation[0][2]);
		}
		else
		{
			eulerAngles.Y = -PI / 2;
			eulerAngles.Z = atan2(-rotation[0][1], -rotation[0][2]);
		}
	}
}


/**
 * \brief Move Transform t to a new position by adding Vec3d pos 
 */
inline void MoveTransform(Transform& t, const Vec3d& pos)
{
	Mat4X4 moveMatrix{};
	moveMatrix[0] = {1, 0, 0, pos.X};
	moveMatrix[1] = {0, 1, 0, pos.Y};
	moveMatrix[2] = {0, 0, 1, pos.Z};
	moveMatrix[3] = {0, 0, 0, 1};
	t.Position = t.Position + pos;
	t.Local2World = t.Local2World * moveMatrix;
	t.World2Local = t.Local2World.Inverse();
}


/**
 * \brief Set the Transform t to new angles based on newRotation 
 */
inline void RotateTransform(Transform& t, const Vec3d& newRotation)
{
	Mat4X4 rotation{};
	Quat q = Euler2Quat(newRotation);
	Quat2Matrix(q, rotation);
	t.Local2World = t.Local2World * rotation;
	t.World2Local = t.Local2World.Inverse();
}


inline void ScalingTransform(Transform& t, const Vec3d newScaling)
{
	Mat4X4 scaleMatrix{};
	scaleMatrix[0] = {newScaling.X, 0, 0, 0};
	scaleMatrix[1] = {0, newScaling.Y, 0, 0};
	scaleMatrix[2] = {0, 0, newScaling.Z, 0};
	scaleMatrix[3] = {0, 0, 0, 1};
	t.Scaling = newScaling;
	t.Local2World = t.Local2World * scaleMatrix;
	t.World2Local = t.Local2World.Inverse();
}

/**
 * \brief Set transform t with rotation r and position pos
 */
inline void SetTransform(Transform& t, const Mat3X3& r, const Vec3d& pos)
{
	t.Position = pos;
	t.Local2World[0] = {r[0][0], r[0][1], r[0][2], pos.X};
	t.Local2World[1] = {r[1][0], r[1][1], r[1][2], pos.Y};
	t.Local2World[2] = {r[2][0], r[2][1], r[2][2], pos.Z};
	t.World2Local = t.Local2World.Inverse();
}

/**
 * \brief Helper function to Initialize empty transform
 */
inline void InitTransform(Transform& t, const Vec3d& pos, const Vec3d& rotation = {0, 0, 0},
                          const Vec3d& scaling = {1, 1, 1})
{
	// setup camera to world matrix
	t.Local2World[0] = {1, 0, 0, 0};
	t.Local2World[1] = {0, 1, 0, 0};
	t.Local2World[2] = {0, 0, 1, 0};
	t.Local2World[3] = {0, 0, 0, 1};
	t.Rotation = Euler2Quat(rotation);
	Quat2Matrix(t.Rotation, t.Local2World);
	ScalingTransform(t, scaling);
	MoveTransform(t, pos);
}
