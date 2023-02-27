#include "stdafx.h"
#include "CameraControl.h"
#include "app/app.h"



void CameraControl::Update(float deltaTime)
{
	Vec3d direction = {};
	double deltaVertical = 0;
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

	if (App::IsKeyPressed('Q'))
	{
		// move down
		deltaVertical = -0.1;
	}

	if (App::IsKeyPressed('E'))
	{
		// move up
		deltaVertical = 0.1;
	}

	RotateCameraTransform(*CamTransform, *Cam, direction, Mouse->DeltaX, Mouse->DeltaY, deltaVertical);
}
