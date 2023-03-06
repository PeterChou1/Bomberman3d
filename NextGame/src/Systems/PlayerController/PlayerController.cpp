#include "stdafx.h"
#include "app/app.h"
#include "PlayerController.h"
#include "Components/Mesh.h"

void PlayerController::Update(float deltaTime)
{
	const double mouseSensitivity = 0.5;
	const double speed = 5 * deltaTime;
	double deltaVertical = 0;
	double deltaMoveX = 0;
	double deltaMoveZ = 0;
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		deltaMoveX = speed;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		deltaMoveX = -speed;
	}

	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		deltaMoveZ = speed;
	}

	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		deltaMoveZ = -speed;
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

	if (App::IsKeyPressed('V'))
	{
		// drop a bomb
		const EntityId meshId = SystemScene.NewEntity();
		const auto mesh = SystemScene.AddComponent<Mesh>(meshId);
		const auto meshTransform = SystemScene.AddComponent<Transform>(meshId);
		const auto meshAABB = SystemScene.AddComponent<AABB>(meshId);
		InitTransform(*meshTransform, PlayerTransform->Position);
		LoadFromObjectFile("./TestData/unitcube.obj", *mesh);
		ComputeAABB(*mesh, *meshAABB);
	}

	double deltaX = mouseSensitivity * Mouse->DeltaX;
	double deltaY = mouseSensitivity * Mouse->DeltaY;
	bool left, right, up, down;
	GetMouseBoundary(left, right, up, down);
	if (left) deltaX = Mouse->ScreenX;
	if (right) deltaX = -Mouse->ScreenX;
	if (up) deltaY = Mouse->ScreenY;
	if (down) deltaY = -Mouse->ScreenY;
	const Vec3d direction = {deltaMoveX, 0, deltaMoveZ};

	if (Cam->Modes == ThirdPerson)
	{
		auto targetTransform = SystemScene.Get<Transform>(Cam->CameraTarget);
		RotateAttachCameraTransform(*CamTransform, *targetTransform, *Cam, deltaX);
		Vec3d move = CamTransform->Forward * deltaMoveZ + CamTransform->Right * deltaMoveX;
		move.Y = 0;
		MoveTransform(*targetTransform, move);
	}
	else if (Cam->Modes == Stationary)
	{
		RotateVector(direction, Cam->AngleH, {0, 1, 0});
		MoveTransform(*PlayerTransform, direction);
		const Vec3d camUp = {0, 1, 0};
		const Vec3d lookAt = PlayerTransform->Position - CamTransform->Position;
		MakeCameraLookAt(*Cam, *CamTransform, CamTransform->Position, camUp, lookAt);
	}
	else if (Cam->Modes == Debug)
	{
		RotateCameraTransform(*CamTransform, *Cam, direction, deltaX, deltaY, deltaVertical);
	}
}
