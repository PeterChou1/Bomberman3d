#include "stdafx.h"
#include "Physics3D.h"

#include <random>

#include "Components/RigidBody.h"
#include "Components/Transform.h"
#include "Components/AABB.h"
#include "Components/PlayerInfoObj.h"
#include "Components/Sound.h"


void Physics3D::Update(const float deltaTime)
{
	using collisionPair = std::pair<EntityId, EntityId>;

	int rigidbody[] = { GetId<Transform>(), GetId<RigidBody>() };
	const auto rigidbodyIterator = SceneIterator(SystemScene, rigidbody, 2);
	for (const EntityId entity : rigidbodyIterator)
	{
		const auto t = SystemScene.Get<Transform>(entity);
		const auto rigidBody = SystemScene.Get<RigidBody>(entity);
		IntegrationStep(*rigidBody, *t, deltaTime);
		MoveTransform(*t, rigidBody->Velocity);
	}

	// 3D collision handling
	int physics3dObj[] = { GetId<Transform>(), GetId<AABB>() };
	const auto aabbIterator = SceneIterator(SystemScene, physics3dObj, 2);
	std::vector<collisionPair> collisionEntityPair;
	std::vector<collisionPair> collisionTriggersPair;

	for (const EntityId A : aabbIterator)
	{
		for (const EntityId B : aabbIterator)
		{
			if (A == B) continue;
			const auto abox = SystemScene.Get<AABB>(A);
			const auto bbox = SystemScene.Get<AABB>(B);
			if (Overlaps(*abox, *bbox) && (abox->Is3D || bbox->Is3D))
			{
				collisionEntityPair.emplace_back(A, B);
			}
		}
	}

	std::sort(collisionEntityPair.begin(), collisionEntityPair.end(),
		[](const collisionPair& p1, const collisionPair& p2)
		{
			if (p1.first != p2.first)
				return p1.first < p2.first;
			return p1.second < p2.second;
		});
	auto last = std::unique(collisionEntityPair.begin(), collisionEntityPair.end());
	collisionEntityPair.erase(last, collisionEntityPair.end());
	std::vector<EntityId> exclude;

	for (auto pair : collisionEntityPair)
	{
		if (std::count(exclude.begin(), exclude.end(), pair.first) ||
			std::count(exclude.begin(), exclude.end(), pair.second)) continue;
		const auto abox = SystemScene.Get<AABB>(pair.first);
		const auto bbox = SystemScene.Get<AABB>(pair.second);
		const auto transformA = SystemScene.Get<Transform>(pair.first);
		const auto transformB = SystemScene.Get<Transform>(pair.second);
		if (abox->IsGrenade ^ bbox->IsGrenade)
		{
			// remove bbox spawn explosion
			const EntityId bomb = SystemScene.NewEntity();
			const auto bombMesh = SystemScene.AddComponent<Mesh>(bomb);
			const auto bombTransform = SystemScene.AddComponent<Transform>(bomb);
			const auto bombAABB = SystemScene.AddComponent<AABB>(bomb);
			Vec3d bombPosition = abox->IsBombTrigger ? transformA->Position : transformB->Position;
			bombPosition.Y = 1;
			bombMesh->color = true;
			bombMesh->r = 1; bombMesh->g = 0; bombMesh->b = 0;
			InitTransform(*bombTransform, bombPosition, {}, { 5, 5, 5 });
			LoadFromObjectFile("./TestData/icosphere.obj", *bombMesh);
			ComputeAABB(*bombMesh, *bombAABB);
			bombAABB->IsBombTrigger = true;
			bombAABB->Is3D = true;
			bombAABB->BombTime = 0.5;
			// add sound effect
			const EntityId soundId = SystemScene.NewEntity();
			const auto soundEffect = SystemScene.AddComponent<SoundEffect>(soundId);
			soundEffect->Soundfile = "./TestData/Test.wav";

			SystemScene.DestroyEntity(abox->IsBombTrigger ? pair.first : pair.second);
			exclude.push_back(abox->IsBombTrigger ? pair.first : pair.second);
		}
		else if ((abox->IsBombTrigger && !bbox->Stationary) ||
			(bbox->IsBombTrigger && !abox->Stationary))
		{
			//EntityId bomb = abox->isBombTrigger && !bbox->Stationary ? pair.first : pair.second;
			EntityId target = abox->IsBombTrigger && !bbox->Stationary ? pair.second : pair.first;
			//AABB* boxBomb = abox->isBombTrigger && !bbox->Stationary ? abox : bbox;
			AABB* boxTarget = abox->IsBombTrigger && !bbox->Stationary ? bbox : abox;
			Transform* bombT = abox->IsBombTrigger && !bbox->Stationary ? transformA : transformB;
			Transform* targetT = abox->IsBombTrigger && !bbox->Stationary ? transformB : transformA;
			
			boxTarget->Is3D = true;
			auto rb = SystemScene.AddComponent<RigidBody>(target);
			Vec3d forceImpulse = targetT->Position - bombT->Position;
			forceImpulse.Y = 25;

			std::random_device rd;
			std::mt19937 gen(rd());
			// Define the range for the random double
			double lower_bound = 25.0;
			double upper_bound = 50.0;
			// Define the distribution and generate the random double
			std::uniform_real_distribution<double> dist(lower_bound, upper_bound);
			double randX = dist(gen);
			double randY = dist(gen);
			double randZ = dist(gen);
			SetUpRigidBody(*rb, *targetT, forceImpulse, { randX, randY, randZ });
		}
		else if ((abox->Stationary && !bbox->Stationary) ||
			(bbox->Stationary && !abox->Stationary))
		{
			EntityId target = abox->Stationary && !bbox->Stationary ? pair.second : pair.first;
			AABB* boxTarget = abox->Stationary && !bbox->Stationary ? bbox : abox;
			Transform* targetT = abox->IsBombTrigger && !bbox->Stationary ? transformB : transformA;
			if (targetT->Position.Y < 0)
			{
				SystemScene.RemoveComponent<Mesh>(target);
				SystemScene.RemoveComponent<AABB>(target);
				SystemScene.RemoveComponent<RigidBody>(target);
				if (boxTarget->IsPlayer)
				{
					// trigger game over
					SystemScene.Get<PlayerInfoObj>(target)->GameOver = true;
				}
				else
				{
					SystemScene.DestroyEntity(target);
				}
				exclude.push_back(target);
			}
		}
	}
}
