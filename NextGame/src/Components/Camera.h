#pragma once
#include <corecrt_math_defines.h>
#include "Transform.h"
#include "ECS/Scene.h"
#include "Math/Vec3d.h"
#include "Math/Matrix.h"

struct Camera
{
	// 3 axis of the camera
	// Vec3d u;
	// Vec3d v;
	// Vec3d w;
	// // camera to world matrix and inverse
	// Mat4x4 cam2world;
	// Mat4x4 world2cam;
	// opengl perspective matrix
	Mat4x4 perspective;
	EntityID cameraTarget;

};

/**
 * \brief Helper function to calculate Opengl perspective matrix parameters
 */
inline void get_perspective(const double& fov_y, const double& aspect_ratio, const double& near_plane,
	double& right, double& left, double& bottom, double& top)
{
	double scale = tan(fov_y * 0.5 * M_PI / 180.0) * near_plane;
	right = aspect_ratio * scale;
	left = -right;
	top = scale;
	bottom = -top;
}

/**
 * \brief Helper function to construct Opengl Perspective Matrix
 */
inline void set_frustum(Camera& cam, const double& near_plane, const double& far_plane,
	const double& right, const double& left, const double& bottom, const double& top)
{
	cam.perspective[0][0] = 2 * near_plane / (right - left);
	cam.perspective[0][1] = 0;
	cam.perspective[0][2] = (right + left) / (right - left);
	cam.perspective[0][3] = 0;
	   
	cam.perspective[1][0] = 0;
	cam.perspective[1][1] = 2 * near_plane / (top - bottom);
	cam.perspective[1][2] = (top + bottom) / (top - bottom);
	cam.perspective[1][3] = 0;
	   
	cam.perspective[2][0] = 0;
	cam.perspective[2][1] = 0;
	cam.perspective[2][2] = -(far_plane + near_plane) / (far_plane - near_plane);
	cam.perspective[2][3] = -2 * far_plane * near_plane / (far_plane - near_plane);
	   
	cam.perspective[3][0] = 0;
	cam.perspective[3][1] = 0;
	cam.perspective[3][2] = -1;
	cam.perspective[3][3] = 0;
}

/**
 * \brief Move Camera to the new camera position
 * \param cam 
 * \param new_pos 
 * \param gaze 
 */
//inline void move_camera_transform(Camera& cam, Transform& new_pos, Vec3d gaze)
//{
//	cam.w = -1 * gaze / gaze.norm();
//	cam.u = new_pos.up.cross(cam.w);
//	cam.u = cam.u / cam.u.norm();
//	cam.v = cam.w.cross(cam.u);
//	// Sanity check for debugging purposes make sure
//	// make sure norm is 1
//	// assert(abs(w.norm() - 1) < std::numeric_limits<double>::epsilon());
//	// assert(abs(u.norm() - 1) < std::numeric_limits<double>::epsilon());
//	// assert(abs(v.norm() - 1) < std::numeric_limits<double>::epsilon());
//	// // camera vector are perpendicular
//	// assert(abs(w.dot(u)) < std::numeric_limits<double>::epsilon());
//	// assert(abs(w.dot(v)) < std::numeric_limits<double>::epsilon());
//	// assert(abs(u.dot(v)) < std::numeric_limits<double>::epsilon());
//	// setup camera to world matrix
//	cam.cam2world[0] = { cam.u.x, cam.v.x, cam.w.x, new_pos.position.x };
//	cam.cam2world[1] = { cam.u.y, cam.v.y, cam.w.y, new_pos.position.y };
//	cam.cam2world[2] = { cam.u.z, cam.v.z, cam.w.z, new_pos.position.z };
//	cam.cam2world[3] = { 0, 0, 0, 1 };
//	cam.world2cam = cam.cam2world.inverse();
//}

//Transform& cam_transform, Vec3d look_at, Vec3d cam_pos, Vec3d up

inline void init_camera(Camera& cam, double angle_y, double near_plane, double far_plane, double aspect_ratio)
{
	// Vec3d gaze = look_at - cam_pos;
	// move_camera_transform(cam, cam_transform, gaze);
	double right, left, bottom, top;
	get_perspective(angle_y, aspect_ratio, near_plane, right, left, bottom, top);
	set_frustum(cam, near_plane, far_plane, right, left, bottom, top);
}