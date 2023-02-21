#include "stdafx.h"
#include "PhysicsWorld.h"

#include "Components/Mesh.h"
#include "Components/PhysicsObj.h"
#include "Components/Transform.h"


void PhysicsWorld::Update(float deltaTime)
{
	// perform a physics time step

	// TODO: refactor scene view
	Vec3d gravity = { 0, -9.81, 0 };
	int componentIds[] = { GetId<Transform>(), GetId<Mesh>(), GetId<PhysicsObj>() };
	auto sceneView = SceneView(m_scene);
	sceneView.init(componentIds, 3);

	for (EntityID entity : sceneView)
	{
		const auto t = m_scene.Get<Transform>(entity);
		const auto phys = m_scene.Get<PhysicsObj>(entity);

		phys->force = phys->force + phys->mass * gravity;

		phys->velocity = phys->velocity + phys->force / phys->mass * deltaTime;

		move_transform(*t, phys->velocity * deltaTime);

		phys->force = { 0, 0, 0 };
	}

}
 