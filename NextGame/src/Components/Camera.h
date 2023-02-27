#pragma once
#include <corecrt_math_defines.h>
#include "Transform.h"
#include "ECS/Scene.h"
#include "Math/Vec3d.h"
#include "Math/Matrix.h"

struct Camera
{
	// horizontal and vertical angle of the camera
	double AngleH;
	double AngleV;
	// where the camera is pointing
	Vec3d Target;

	Vec3d Up;
	// OpenGL perspective matrix
	Mat4X4 Perspective;
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

/**
 * \brief Set transform t to pos with orientation set by top and gaze
 * \param t transform to set
 * \param pos new position of the transform
 * \param top vector sets the up vector (up vector is always perpendicular to the forward and top vector)
 * \param target vector which sets forward vector (the forward vector is points in the opposite direction of gaze)
 */
inline void SetCameraTransform(Transform& t, const Vec3d& pos, const Vec3d& top, const Vec3d& target)
{
	const Vec3d forward = -1 * target / Mag(target);
	Vec3d right = Cross(top, forward);
	right = right / Mag(right);
	const Vec3d up = Cross(forward, right);
	t.Position = pos;
	// setup camera to world matrix
	t.Local2World[0] = {right.X, up.X, forward.X, pos.X};
	t.Local2World[1] = {right.Y, up.Y, forward.Y, pos.Y};
	t.Local2World[2] = {right.Z, up.Z, forward.Z, pos.Z};
	t.Local2World[3] = {0, 0, 0, 1};
	Matrix2Quat(t.Local2World, t.Rotation);
	t.World2Local = t.Local2World.Inverse();
}

inline void InitCamera(Camera& cam, const double angleY, const double nearPlane, const double farPlane, const double aspectRatio)
{
	double right, left, bottom, top;
	GetPerspective(angleY, aspectRatio, nearPlane, right, left, bottom, top);
	SetFrustum(cam, nearPlane, farPlane, right, left, bottom, top);
}

inline void InitCamera(Camera& cam, Transform& t, const Vec3d& pos, const Vec3d& up, const Vec3d& target,
                       const double angleY, const double nearPlane, const double farPlane, const double aspectRatio)
{
	// Find Angle of Target Vector on X, Z plane
	Vec3d targetNorm = Norm(target);
	Vec3d htarget = { targetNorm.X, 0, targetNorm.Z};
	Normalize(htarget);
	// Get Angle between htarget and view
	// split into 4 quadrants
	float angle = acos(targetNorm.X);
	if (htarget.Z >= 0.0f)
	{
		cam.AngleH = htarget.X >= 0.0 ? 2 * PI - angle : PI + angle;
	}
	else
	{
		cam.AngleH = htarget.X >= 0.0 ? angle : PI - angle;
	}
	cam.AngleV = -asin(targetNorm.Y);
	cam.Target = targetNorm;
	cam.Up = up;
	SetCameraTransform(t, pos, up, target);
	double right, left, bottom, top;
	GetPerspective(angleY, aspectRatio, nearPlane, right, left, bottom, top);
	SetFrustum(cam, nearPlane, farPlane, right, left, bottom, top);
}

/**
 * \brief Move Transform t to a new position by adding Vec3d pos and Rotated by rot
 */
inline void RotateCameraTransform(Transform& t, Camera& cam, const Vec3d& pos, const double deltaX, const double deltaY, const double deltaUp)
{
	Vec3d moveUp = { 0, deltaUp, 0 };
	// rotate
	cam.AngleH += deltaX;
	cam.AngleV += deltaY;
	Vec3d Yaxis = { 0, 1, 0 };
	Vec3d target = { 1, 0, 0 };
	target = RotateVector(target, cam.AngleH, Yaxis);
	Normalize(target);
	Vec3d u = Cross(Yaxis, target);
	Normalize(u);
	target = RotateVector(target, cam.AngleV, u);
	Normalize(target);
	Vec3d up = Cross(target, u);
	Normalize(up);
	SetCameraTransform(t, t.Position + moveUp, up, target);
	cam.Up = up;
	cam.Target = target;

	Mat4X4 moveMatrix{};
	moveMatrix[0] = { 1, 0, 0, pos.X };
	moveMatrix[1] = { 0, 1, 0, pos.Y };
	moveMatrix[2] = { 0, 0, 1, pos.Z };
	moveMatrix[3] = { 0, 0, 0, 1 };

	t.Local2World = t.Local2World * moveMatrix;
	t.World2Local = t.Local2World.Inverse();
	t.Position = { t.Local2World[0][3], t.Local2World[1][3], t.Local2World[2][3] };

}