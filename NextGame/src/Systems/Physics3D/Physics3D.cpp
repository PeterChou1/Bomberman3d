#include "stdafx.h"
#include "Physics3D.h"
#include "Components/RigidBody.h"
#include "Components/Transform.h"



void Physics3D::Update(const float deltaTime)
{
	int rigidbody[] = { GetId<Transform>(), GetId<RigidBody>() };
	const auto rigidbodyIterator = SceneIterator(SystemScene, rigidbody, 2);
	for (const EntityId entity : rigidbodyIterator)
	{
		const auto t = SystemScene.Get<Transform>(entity);
		const auto rigidBody = SystemScene.Get<RigidBody>(entity);
		IntegrationStep(*rigidBody, *t, deltaTime);
		MoveTransform(*t, rigidBody->Velocity);
	}
}