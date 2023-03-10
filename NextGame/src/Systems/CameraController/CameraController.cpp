#include "stdafx.h"
#include "CameraController.h"
#include "app/app.h"


void CameraController::Update(const float deltaTime)
{
	// --- (Camera Logic) ---
	if (Cam->Modes == Camera::ThirdPerson)
	{
		double deltaX = mouseSensitivity * Mouse->DeltaX;
		bool left, right, up, down;
		GetMouseBoundary(left, right, up, down);
		if (left) deltaX = Mouse->ScreenX;
		if (right) deltaX = -Mouse->ScreenX;
		auto targetTransform = SystemScene.Get<Transform>(Cam->CameraTarget);
		RotateAttachCameraTransform(*CamTransform, *targetTransform, *Cam, deltaX);
	}
	else if(Cam->Modes == Camera::Debug)
	{
		const double mouseSensitivity = 0.5;
		const double speed = 10 * deltaTime;
		double deltaVertical = 0;
		double deltaMoveX = 0;
		double deltaMoveZ = 0;

		if (App::GetController().GetLeftThumbStickX() > 0.5f)
		{
			deltaMoveX = -speed;
		}
		if (App::GetController().GetLeftThumbStickX() < -0.5f)
		{
			deltaMoveX = speed;
		}

		if (App::GetController().GetLeftThumbStickY() > 0.5f)
		{
			deltaMoveZ = -speed;
		}

		if (App::GetController().GetLeftThumbStickY() < -0.5f)
		{
			deltaMoveZ = speed;
		}

		if (App::IsKeyPressed('Q'))
		{
			// move down
			deltaVertical = -speed;
		}

		if (App::IsKeyPressed('E'))
		{
			// move up
			deltaVertical = speed;
		}
		double deltaX = mouseSensitivity * Mouse->DeltaX;
		double deltaY = mouseSensitivity * Mouse->DeltaY;
		bool left, right, up, down;
		GetMouseBoundary(left, right, up, down);
		if (left) deltaX = Mouse->ScreenX;
		if (right) deltaX = -Mouse->ScreenX;
		if (up) deltaY = Mouse->ScreenY;
		if (down) deltaY = -Mouse->ScreenY;
		const Vec3d direction = { deltaMoveX, 0, deltaMoveZ };
		RotateCameraTransform(*CamTransform, *Cam, direction, deltaX, deltaY, deltaVertical);

	}
}
