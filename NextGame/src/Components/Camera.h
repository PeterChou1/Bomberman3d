#pragma once
#include <corecrt_math_defines.h>
#include "Transform.h"
#include "ECS/Scene.h"
#include "Math/Vec3d.h"
#include "Math/Matrix.h"

struct Camera
{
	// OpenGL perspective matrix
	Mat4x4 Perspective;
	EntityId CameraTarget;
};

/**
 * \brief Helper function to calculate OpenGl perspective matrix parameters
 */
inline void GetPerspective(const double fovY, const double aspectRatio, const double nearPlane,
	double& right, double& left, double& bottom, double& top)
{
	const double scale = tan(fovY * 0.5 * M_PI / 180.0) * nearPlane;
	right = aspectRatio * scale;
	left = -right;
	top = scale;
	bottom = -top;
}

/**
 * \brief Helper function to construct OpenGl Perspective Matrix
 */
inline void SetFrustum(Camera& cam, const double nearPlane, const double& farPlane,
	const double right, const double left, const double bottom, const double top)
{
	cam.Perspective[0][0] = 2 * nearPlane / (right - left);
	cam.Perspective[0][1] = 0;
	cam.Perspective[0][2] = (right + left) / (right - left);
	cam.Perspective[0][3] = 0;
	   
	cam.Perspective[1][0] = 0;
	cam.Perspective[1][1] = 2 * nearPlane / (top - bottom);
	cam.Perspective[1][2] = (top + bottom) / (top - bottom);
	cam.Perspective[1][3] = 0;
	   
	cam.Perspective[2][0] = 0;
	cam.Perspective[2][1] = 0;
	cam.Perspective[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
	cam.Perspective[2][3] = -2 * farPlane * nearPlane / (farPlane - nearPlane);
	   
	cam.Perspective[3][0] = 0;
	cam.Perspective[3][1] = 0;
	cam.Perspective[3][2] = -1;
	cam.Perspective[3][3] = 0;
}


inline void InitCamera(Camera& cam, const double angleY, const double nearPlane, const double farPlane, const double aspectRatio)
{
	// Vec3d gaze = look_at - cam_pos;
	// set_transform(cam, cam_transform, gaze);
	double right, left, bottom, top;
	GetPerspective(angleY, aspectRatio, nearPlane, right, left, bottom, top);
	SetFrustum(cam, nearPlane, farPlane, right, left, bottom, top);
}