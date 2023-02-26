#include "stdafx.h"
#include "CameraControl.h"
#include "app/app.h"



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
	Vec3d curRot = Quat2Euler(CamTransform->Rotation);
	//const Vec3d rotation = { mouseY, -mouseX, 0};
	const Vec3d rotation = { 0, 0, 0 };
	MoveRotateTransform(*CamTransform, direction, rotation * deltaTime);
}
