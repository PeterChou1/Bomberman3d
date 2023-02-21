#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "CameraControl.h"

#include "app/app.h"

void CameraControl::Update(float deltaTime)
{
	Vec3d directionX = { 0.1, 0, 0 };
	Vec3d new_position = cam_transform->position;

	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		directionX = cam_transform->local2world * directionX;
		new_position = directionX;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		directionX = -1 * directionX;
		directionX = cam_transform->local2world * directionX;
		new_position = directionX;
	}

	Vec3d directionZ = { 0, 0, 0.1 };
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		directionZ = cam_transform->local2world * directionZ;
		new_position = directionZ;
	}
	//
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		directionZ = -1 * directionZ;
		directionZ = cam_transform->local2world * directionZ;
		new_position = directionZ;
	}
	Vec3d look_at = { 0, 0, 0 };
	Vec3d gaze = look_at - new_position;
	move_camera_transform(*cam_transform, new_position, cam_transform->up, gaze);
}
