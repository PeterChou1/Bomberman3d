#include "stdafx.h"
#include "app/app.h"
#include "PlayerController.h"

#include "Components/Bomb.h"
#include "Components/Map.h"
#include "Components/Mesh.h"

void PlayerController::Update(float deltaTime)
{
	// --- (Game logic) ---
	if (PlayerInfo->GameOver || PlayerInfo->Debug) return;

	if (PlayerInfo->CurrentCooldown > 0)
	{
		PlayerInfo->CurrentCooldown -= deltaTime;
	}
	if (PlayerInfo->CurrentCooldown <= 0)
	{
		PlayerInfo->CurrentCooldown = 0;
	}

	if (App::IsKeyPressed('E'))
	{
		PlayerInfo->GrenadeMode = true;
	}
	else
	{
		PlayerInfo->GrenadeMode = false;
	}

	if (App::IsKeyPressed(' ') && PlayerInfo->CurrentCooldown <= 0 && !PlayerInfo->GrenadeMode)
	{
		// verify we can place a bombs
		Vec3d bombPosition = { std::round(PlayerTransform->Position.X), bombY, std::round(PlayerTransform->Position.Z) };
		Vec3d forward = Norm(Cross({ 0, 1, 0 }, CamTransform->Right));
		bombPosition = bombPosition + forward;
		bombPosition.X = std::round(bombPosition.X);
		bombPosition.Z = std::round(bombPosition.Z);

		if (Vec3toBlockMap(SceneMap->BlockMap, bombPosition) == 1) {
			return;
		}

		// Construct and drop a bomb
		const EntityId meshId = SystemScene.NewEntity();
		const auto mesh = SystemScene.AddComponent<Mesh>(meshId);
		const auto meshTransform = SystemScene.AddComponent<Transform>(meshId);
		const auto meshAABB = SystemScene.AddComponent<AABB>(meshId);
		SystemScene.AddComponent<Bomb>(meshId);
		mesh->color = true;
		mesh->r = bombColorR; mesh->g = bombColorG; mesh->b = bombColorB;
		bombPosition.Y = bombY;
		InitTransform(*meshTransform, bombPosition);
		LoadFromObjectFile("./TestData/unitcube.obj", *mesh);
		ComputeAABB(*mesh, *meshAABB);
		PlayerInfo->CurrentCooldown = PlayerInfo->BombCooldown;
	}
	// can't move in grenade mode return early
	if (PlayerInfo->GrenadeMode) return;

	// --- (Player movement logic) ---
	const double speed = playerSpeed * deltaTime;
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
	Vec3d move = CamTransform->Forward * deltaMoveZ + CamTransform->Right * deltaMoveX;
	move.Y = 0;
	MoveTransform(*PlayerTransform, move);
}
