#include "stdafx.h"
#include "CameraControl.h"
#include "app/app.h"


void GetMouseAxis()
{
	
}


void CameraControl::Update(float deltaTime)
{
	Vec3d direction = {};

	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		direction.X = 0.1;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		direction.X = -0.1;
	}

	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		direction.Z = 0.1;
	}
	
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		direction.Z = -0.1;
	}
	float mouseX, mouseY;
	App::GetMouseAxis(mouseX, mouseY);

	const Vec3d rotation = { mouseY, -1 * mouseX, 0};
	MoveRotateTransform(*CamTransform, direction, rotation * deltaTime);
}
