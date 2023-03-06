#include "stdafx.h"
#include "Physics3D.h"

#include "Components/Mesh.h"
#include "Components/Particle.h"
#include "Components/RigidBody.h"
#include "Components/Transform.h"
#include "Components/AABB.h"


void Physics3D::Update(const float deltaTime)
{

	// STEP 1: transform aabb
	//int collisonTransform[] = { GetId<AABB>(), GetId<Transform>() };
	//const auto collisionObj = SceneIterator(SystemScene, collisonTransform, 2);
	//std::vector<std::pair<EntityId, EntityId>> collisionPair;
	//std::vector<EntityId> ids;
	//for (const EntityId cObj : collisionObj)
	//{
	//	auto cTransform  = SystemScene.Get<Transform>(cObj);
	//	auto aabb = SystemScene.Get<AABB>(cObj);
	//	TransformAABB(*aabb, *cTransform);
	//}


	////// STEP 2: test for collision
	//int aabb[] = { GetId<AABB>(), GetId<Transform>() };
	//const auto aabbIterator = SceneIterator(SystemScene, aabb, 2);
	//
	//for (const EntityId A : aabbIterator)
	//{
	//	for (const EntityId B : aabbIterator)
	//	{
	//		if (A == B) continue;
	//		const auto aabbEntityA = SystemScene.Get<AABB>(A);
	//		const auto aabbEntityB = SystemScene.Get<AABB>(B);
	//
	//		if (Overlaps(*aabbEntityA, *aabbEntityB))
	//		{
	//			collisionPair.emplace_back(std::make_pair(A, B));
	//			ids.push_back(A);
	//			ids.push_back(B);
	//		}
	//	}
	//}
	//int mesh[] = { GetId<Mesh>() };
	//const auto meshIterator = SceneIterator(SystemScene, mesh, 1);

	////// STEP 3: mark non collided mesh as not collided
	//for (auto id : meshIterator)
	//{
	//	auto it = std::find(ids.begin(), ids.end(), id);
	//	if (it == ids.end())
	//	{
	//		const auto mesh = SystemScene.Get<Mesh>(id);
	//		mesh->collided = false;
	//	}
	//}

	////// STEP 4: Narrow phase collision
	//for (auto pair : collisionPair)
	//{
	//	const auto meshA = SystemScene.Get<Mesh>(pair.first);
	//	const auto meshB = SystemScene.Get<Mesh>(pair.second);
	//	meshA->collided = true;
	//	meshB->collided = true;
	//}

	//int rigidbody[] = { GetId<Transform>(), GetId<RigidBody>() };
	//const auto rigidbodyIterator = SceneIterator(SystemScene, rigidbody, 2);
	//
	//for (const EntityId entity : rigidbodyIterator)
	//{
	//	const auto t = SystemScene.Get<Transform>(entity);
	//	const auto rigidBody = SystemScene.Get<RigidBody>(entity);
	//	// RotateTransform(*t, { 0.01, 0.1, 0.05});
	//	IntegrationStep(*rigidBody, *t, deltaTime);
	//	// MoveTransform(*t, rigidBody->Velocity);
	//}
	// 
}