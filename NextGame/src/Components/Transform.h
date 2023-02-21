#pragma once

#include "Math/Matrix.h"
#include "Math/Vec3d.h"

struct Transform
{
	// x, y, z coordinates
	Vec3d position;
	// euler angles rotation
	Vec3d rotation;
	// scaling factor
	Vec3d scaling;

	Mat4x4 local2world;

	Mat4x4 world2local;

	Vec3d up;
};

/**
 * \brief Move Transform t to a new position by adding Vec3d pos 
 * \param t transform to move
 * \param pos position that is added to the transform
 */
inline void move_transform(Transform& t, Vec3d pos)
{
	Mat4x4 moveMatrix;
	moveMatrix[0] = { 1, 0, 0, pos.x };
	moveMatrix[1] = { 0, 1, 0, pos.y };
	moveMatrix[2] = { 0, 0, 1, pos.z };
	moveMatrix[3] = { 0, 0, 0, 1 };

	t.position = pos;
	t.local2world = t.local2world * moveMatrix;
	t.world2local = t.local2world.inverse();
}

inline void rotate_transform(Transform& t, Vec3d new_rotation)
{
	Mat4x4 rotationX;
	Mat4x4 rotationZ;
	Mat4x4 rotationY;
	rotationX[0] = { 1, 0, 0, 0 };
	rotationX[1] = { 0, cos(new_rotation.x), -sin(new_rotation.x), 0 };
	rotationX[2] = { 0, sin(new_rotation.x), cos(new_rotation.x), 0 };
	rotationX[3] = { 0, 0, 0, 1 };

	rotationY[0] = { cos(new_rotation.y), 0, sin(new_rotation.y), 0 };
	rotationY[1] = { 0, 1, 0, 0 };
	rotationY[2] = { -sin(new_rotation.y), 0, cos(new_rotation.y), 0 };
	rotationY[3] = { 0, 0, 0, 1 };
			
	rotationZ[0] = { cos(new_rotation.z), -sin(new_rotation.z), 0, 0 };
	rotationZ[1] = { sin(new_rotation.z),  cos(new_rotation.z), 0, 0 };
	rotationZ[2] = { 0, 0, 1, 0 };
	rotationZ[3] = { 0, 0, 0, 1 };

	t.rotation = new_rotation;
	t.local2world = t.local2world * rotationX * rotationY * rotationZ;
	t.world2local = t.local2world.inverse();
}


inline void scaling_transform(Transform& t, Vec3d new_scaling)
{
	Mat4x4 moveMatrix;
	moveMatrix[0] = { new_scaling.x, 0, 0, 0};
	moveMatrix[1] = { 0, new_scaling.y, 0, 0};
	moveMatrix[2] = { 0, 0, new_scaling.z, 0};
	moveMatrix[3] = { 0, 0, 0, 1 };
	t.scaling = new_scaling;
	t.local2world = t.local2world * moveMatrix;
	t.world2local = t.local2world.inverse();
}

inline void move_camera_transform(Transform& t, Vec3d pos, Vec3d up, Vec3d gaze)
{
	Vec3d w = -1 * gaze / gaze.norm();
	Vec3d u = up.cross(w);
	u = u / u.norm();
	Vec3d v = w.cross(u);
	t.up = up;
	t.position = pos;
	// setup camera to world matrix
	t.local2world[0] = { u.x, v.x, w.x, pos.x };
	t.local2world[1] = { u.y, v.y, w.y, pos.y };
	t.local2world[2] = { u.z, v.z, w.z, pos.z };
	t.local2world[3] = { 0, 0, 0, 1 };
	t.world2local = t.local2world.inverse();
}

inline void init_transform(Transform& t, Vec3d pos, Vec3d rotation = { 0, 0, 0 }, Vec3d scaling = {1, 1, 1})
{
	// setup camera to world matrix
	t.local2world[0] = { 1, 0, 0, 0 };
	t.local2world[1] = { 0, 1, 0, 0 };
	t.local2world[2] = { 0, 0, 1, 0 };
	t.local2world[3] = { 0, 0, 0, 1 };
	rotate_transform(t, rotation);
	scaling_transform(t, scaling);
	move_transform(t, pos);
}