#include "stdafx.h"
#include "GrenadeHandler.h"
#include "Components/GernadeTrajectory.h"
#include "Components/RigidBody.h"


/**
 * \brief Takes vector a and outputs coords in screen space of sceneCam
 *        returns false if point is not on screen
 */
inline bool ToScreenSpace(const Vec3d a, Vec3d& coords, const Camera& sceneCam, const Transform& camTransform, const Display& display)
{
	Vec3d proj = sceneCam.Perspective * camTransform.World2Local * a;
	coords.X = (proj.X + display.AspectRatio) * 0.5 * APP_VIRTUAL_WIDTH;
	coords.Y = (proj.Y + 1) * 0.5 * APP_VIRTUAL_HEIGHT;
	coords.Z = proj.Z;

	if (proj.X < -display.AspectRatio || proj.X > display.AspectRatio ||
		proj.Y < -1 || proj.Y > 1 || proj.Z < -1 || proj.Z > 1)
		return false;

	return true;
}

void GrenadeHandler::Update(float deltaTime)
{
	// --- (Game logic) ---
	if (PlayerInfo->GrenadeMode)
	{
		// Map mouse screen to world
		EntityId grenadeTrajectoryEntity = SystemScene.GetEntityWithComponent<GernadeTrajectory>();
		GernadeTrajectory* trajectory;
		if (!IsEntityValid(grenadeTrajectoryEntity))
		{
			grenadeTrajectoryEntity = SystemScene.NewEntity();
			trajectory = SystemScene.AddComponent<GernadeTrajectory>(grenadeTrajectoryEntity);
		} else
		{
			trajectory = SystemScene.Get<GernadeTrajectory>(grenadeTrajectoryEntity);
		}
		const Vec3d forward = Cross(Cam->Up, CamTransform->Right);
		trajectory->CurrentPower += deltaTime * trajectory->PowerGrowth;
		if (trajectory->CurrentPower >= trajectory->FullPower)
		{
			trajectory->CurrentPower = trajectory->FullPower;
		}
		trajectory->Start = PlayerTransform->Position;
		trajectory->End = PlayerTransform->Position + CamTransform->Up * trajectory->CurrentPower + forward * trajectory->CurrentPower;
	}
	else
	{
		// previously held down spawn a generade component
		const EntityId grenadeTrajectoryEntity = SystemScene.GetEntityWithComponent<GernadeTrajectory>();
		if (IsEntityValid(grenadeTrajectoryEntity))
		{
			auto trajectory = SystemScene.Get<GernadeTrajectory>(grenadeTrajectoryEntity);

			const EntityId grenadeEntityId = SystemScene.NewEntity();
			const auto grenadeMesh = SystemScene.AddComponent<Mesh>(grenadeEntityId);
			const auto grenadeTransform = SystemScene.AddComponent<Transform>(grenadeEntityId);
			const auto rigidBody = SystemScene.AddComponent<RigidBody>(grenadeEntityId);
			const auto aabb = SystemScene.AddComponent<AABB>(grenadeEntityId);
			Vec3d Force = 5 * (trajectory->End - trajectory->Start);
			SetUpRigidBody(*rigidBody, *grenadeTransform, Force, {50, 25, 10});
			InitTransform(*grenadeTransform, trajectory->End, {}, {0.5, 0.5, 0.5});
			LoadFromObjectFile("./TestData/unitcube.obj", *grenadeMesh);
			grenadeMesh->color = true;
			grenadeMesh->r = 0.5;
			grenadeMesh->g = 0.5;
			grenadeMesh->b = 0.5;
			ComputeAABB(*grenadeMesh, *aabb);
			aabb->Is3D = true;
			aabb->IsGrenade = true;
			SystemScene.DestroyEntity(grenadeTrajectoryEntity);
		}
	}
}



void GrenadeHandler::Render()
{
	if (PlayerInfo->GrenadeMode)
	{
		EntityId grenadeEntityId = SystemScene.GetEntityWithComponent<GernadeTrajectory>();
		const GernadeTrajectory* trajectory = SystemScene.Get<GernadeTrajectory>(grenadeEntityId);
		if (IsEntityValid(grenadeEntityId))
		{
			Vec3d pt1{};
			ToScreenSpace(trajectory->Start, pt1, *Cam, *CamTransform, *Display);
			Vec3d pt2{};
			ToScreenSpace(trajectory->End, pt2, *Cam, *CamTransform, *Display);
			App::DrawLine(pt1.X, pt1.Y, pt2.X, pt2.Y, 1, 0, 0);
		}
	}
}