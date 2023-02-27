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

	double deltaX = Mouse->DeltaX;
	double deltaY = Mouse->DeltaY;
	bool left, right, up, down;
	App::GetMouseBoundary(left, right, up, down);

	if (left) deltaX = Mouse->ScreenX;
	if (right) deltaX = -Mouse->ScreenX;
	if (up) deltaY = Mouse->ScreenY;
	if (down) deltaY = -Mouse->ScreenY;

	RotateCameraTransform(*CamTransform, *Cam, direction, deltaX, deltaY, deltaVertical);
}
