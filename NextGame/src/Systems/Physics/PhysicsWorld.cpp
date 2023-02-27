#include "stdafx.h"
#include "PhysicsWorld.h"

#include "Components/Mesh.h"
#include "Components/Particle.h"
#include "Components/RigidBody.h"
#include "Components/Transform.h"


//void ComputeForces(Scene& scene, Particle& particle, const Transform& particleTransform)
//{
//	// TODO: predefined constants (move it globally?)
//	const Vec3d gravity = {0, -9.81, 0};
//	const double kdrag = 1;
//	Vec3d forceApplied = {0, 0, 0};
//
//	if (particle.Gravity)
//	{
//		forceApplied = forceApplied + particle.Mass * gravity + -1 * kdrag * particle.Velocity;
//	}
//
//	if (particle.SpringMass)
//	{
//		const auto partnerParticle = scene.Get<Particle>(particle.Partner);
//		const auto partnerTransform = scene.Get<Transform>(particle.Partner);
//		const auto posDelta = particleTransform.Position - partnerTransform->Position;
//		const Vec3d vDelta = particle.Velocity - partnerParticle->Velocity;
//		const double distance = Mag(posDelta);
//		const Vec3d springForce = -1 * (particle.SpringConstant * (distance - particle.RestLength) + particle.
//			DampingConstant * (Dot(vDelta, posDelta) / distance)) * (posDelta / distance);
//		partnerParticle->Force = partnerParticle->Force + -1 * springForce;
//	}
//	particle.Force = particle.Force + forceApplied;
//}
//
//void PhysicsWorld::Update(const float deltaTime)
//{
//	int componentIds[] = {GetId<Transform>(), GetId<Mesh>(), GetId<Particle>()};
//	const auto sceneIterator = SceneIterator(SystemScene, componentIds, 3);
//
//	for (const EntityId entity : sceneIterator)
//	{
//		const auto t = SystemScene.Get<Transform>(entity);
//		const auto phys = SystemScene.Get<Particle>(entity);
//		ComputeForces(SystemScene, *phys, *t);
//		// Time step using Euler method (can be improved)
//		phys->Velocity = phys->Velocity + phys->Force / phys->Mass * deltaTime;
//		MoveTransform(*t, phys->Velocity);
//		phys->Force = {0, 0, 0};
//	}
//}


void PhysicsWorld::Update(const float deltaTime)
{
	int componentIds[] = { GetId<Transform>(), GetId<RigidBody>() };
	const auto sceneIterator = SceneIterator(SystemScene, componentIds, 2);
	
	for (const EntityId entity : sceneIterator)
	{
		const auto t = SystemScene.Get<Transform>(entity);
		const auto rigidBody = SystemScene.Get<RigidBody>(entity);
		// RotateTransform(*t, { 0.01, 0.1, 0.05});
		IntegrationStep(*rigidBody, *t, deltaTime);
		// MoveTransform(*t, rigidBody->Velocity);
	}

	// 
}