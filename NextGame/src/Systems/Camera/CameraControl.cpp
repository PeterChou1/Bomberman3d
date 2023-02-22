#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "CameraControl.h"

#include "app/app.h"

void CameraControl::Update(float deltaTime)
{
	Vec3d directionX = {0.1, 0, 0};
	Vec3d newPosition = CamTransform->Position;

	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		directionX = CamTransform->Local2World * directionX;
		newPosition = directionX;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		directionX = -1 * directionX;
		directionX = CamTransform->Local2World * directionX;
		newPosition = directionX;
	}

	Vec3d directionZ = {0, 0, 0.1};
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		directionZ = CamTransform->Local2World * directionZ;
		newPosition = directionZ;
	}
	//
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		directionZ = -1 * directionZ;
		directionZ = CamTransform->Local2World * directionZ;
		newPosition = directionZ;
	}
	constexpr Vec3d lookAt = {0, 0, 0};
	const Vec3d gaze = lookAt - newPosition;
	SetTransform(*CamTransform, newPosition, CamTransform->Up, gaze);
}
