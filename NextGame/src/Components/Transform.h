#pragma once

#include "Math/Matrix.h"
#include "Math/Vec3d.h"


struct Quaternion
{
	
};

struct Transform
{
	// x, y, z coordinates
	Vec3d Position;
	// euler angles rotation
	// Vec3d Rotation;
	// scaling factor
	Vec3d Scaling;

	Mat4X4 Local2World;

	Mat4X4 World2Local;

	Vec3d Up;
};

inline Mat3X3 GetRotationMatrix(const Transform& t)
{
	Mat3X3 rotation{};
	rotation[0] = { t.Local2World[0][0], t.Local2World[0][1], t.Local2World[0][2] };
	rotation[1] = { t.Local2World[1][0], t.Local2World[1][1], t.Local2World[1][2] };
	rotation[2] = { t.Local2World[2][0], t.Local2World[2][1], t.Local2World[2][2] };
	return rotation;
}



/**
 * \brief Move Transform t to a new position by adding Vec3d pos 
 * \param t transform to move
 * \param pos position that is added to the transform
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

inline void RotateTransform(Transform& t, const Vec3d& newRotation)
{
	Mat4X4 rotationX{};
	Mat4X4 rotationZ{};
	Mat4X4 rotationY{};
	rotationX[0] = {1, 0, 0, 0};
	rotationX[1] = {0, cos(newRotation.X), -sin(newRotation.X), 0};
	rotationX[2] = {0, sin(newRotation.X), cos(newRotation.X), 0};
	rotationX[3] = {0, 0, 0, 1};

	rotationY[0] = {cos(newRotation.Y), 0, sin(newRotation.Y), 0};
	rotationY[1] = {0, 1, 0, 0};
	rotationY[2] = {-sin(newRotation.Y), 0, cos(newRotation.Y), 0};
	rotationY[3] = {0, 0, 0, 1};

	rotationZ[0] = {cos(newRotation.Z), -sin(newRotation.Z), 0, 0};
	rotationZ[1] = {sin(newRotation.Z), cos(newRotation.Z), 0, 0};
	rotationZ[2] = {0, 0, 1, 0};
	rotationZ[3] = {0, 0, 0, 1};

	t.Local2World = t.Local2World * rotationX * rotationY * rotationZ;
	t.World2Local = t.Local2World.Inverse();
}


inline void ScalingTransform(Transform& t, const Vec3d newScaling)
{
	Mat4X4 moveMatrix{};
	moveMatrix[0] = {newScaling.X, 0, 0, 0};
	moveMatrix[1] = {0, newScaling.Y, 0, 0};
	moveMatrix[2] = {0, 0, newScaling.Z, 0};
	moveMatrix[3] = {0, 0, 0, 1};
	t.Scaling = newScaling;
	t.Local2World = t.Local2World * moveMatrix;
	t.World2Local = t.Local2World.Inverse();
}

/**
 * \brief Set transform t to pos with orientation set by top and gaze 
 * \param t transform to set
 * \param pos new position of the transform
 * \param top vector sets the up vector (up vector is always perpendicular to the forward and top vector)
 * \param gaze vector which sets forward vector (the forward vector is points in the opposite direction of gaze)
 */
inline void SetTransform(Transform& t, const Vec3d& pos, const Vec3d& top, const Vec3d& gaze)
{
	const Vec3d forward = -1 * gaze / Mag(gaze);
	Vec3d right = Cross(top, forward);
	right = right / Mag(right);
	const Vec3d up = Cross(forward, right);

	t.Up = up;
	t.Position = pos;
	// setup camera to world matrix
	t.Local2World[0] = {right.X, up.X, forward.X, pos.X};
	t.Local2World[1] = {right.Y, up.Y, forward.Y, pos.Y};
	t.Local2World[2] = {right.Z, up.Z, forward.Z, pos.Z};
	t.Local2World[3] = {0, 0, 0, 1};
	t.World2Local = t.Local2World.Inverse();
}


inline void SetTransform(Transform& t, const Mat3X3& rotation, const Vec3d& pos)
{
	t.Position = pos;
	t.Local2World[0] = { rotation[0][0], rotation[0][1], rotation[0][2], pos.X };
	t.Local2World[1] = { rotation[1][0], rotation[1][1], rotation[1][2], pos.Y };
	t.Local2World[2] = { rotation[2][0], rotation[2][1], rotation[2][2], pos.Z };
	t.World2Local = t.Local2World.Inverse();
}

inline void InitTransform(Transform& t, const Vec3d& pos, const Vec3d& rotation = {0, 0, 0}, const Vec3d& scaling = {1, 1, 1})
{
	// setup camera to world matrix
	t.Local2World[0] = {1, 0, 0, 0};
	t.Local2World[1] = {0, 1, 0, 0};
	t.Local2World[2] = {0, 0, 1, 0};
	t.Local2World[3] = {0, 0, 0, 1};
	RotateTransform(t, rotation);
	ScalingTransform(t, scaling);
	MoveTransform(t, pos);
}
