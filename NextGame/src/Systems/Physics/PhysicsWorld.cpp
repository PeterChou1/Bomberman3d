#include "stdafx.h"
#include "PhysicsWorld.h"

#include "Components/Mesh.h"
#include "Components/ParticleObj.h"
#include "Components/Transform.h"


void ComputeForces(Scene& scene, ParticleObj& particle, const Transform& particleTransform)
{
	// TODO: predefined constants (move it globally?)
	const Vec3d gravity = {0, -9.81, 0};
	const double kdrag = 1;
	Vec3d forceApplied = {0, 0, 0};

	if (particle.Gravity)
	{
		forceApplied = forceApplied + particle.Mass * gravity + -1 * kdrag * particle.Velocity;
	}

	if (particle.SpringMass)
	{
		const auto partnerParticle = scene.Get<ParticleObj>(particle.Partner);
		const auto partnerTransform = scene.Get<Transform>(particle.Partner);
		const auto posDelta = particleTransform.Position - partnerTransform->Position;
		const Vec3d v_delta = particle.Velocity - partnerParticle->Velocity;
		const double distance = Mag(posDelta);
		const Vec3d spring_force = -1 * (particle.SpringConstant * (distance - particle.RestLength) + particle.
			DampingConstant * (Dot(v_delta, posDelta) / distance)) * (posDelta / distance);
		partnerParticle->Force = partnerParticle->Force + -1 * spring_force;
	}
	particle.Force = particle.Force + forceApplied;
}

void PhysicsWorld::Update(float deltaTime)
{
	int componentIds[] = {GetId<Transform>(), GetId<Mesh>(), GetId<ParticleObj>()};
	const auto sceneIterator = SceneIterator(SystemScene, componentIds, 3);

	for (const EntityId entity : sceneIterator)
	{
		const auto t = SystemScene.Get<Transform>(entity);
		const auto phys = SystemScene.Get<ParticleObj>(entity);
		ComputeForces(SystemScene, *phys, *t);
		phys->Velocity = phys->Velocity + phys->Force / phys->Mass * deltaTime;
		MoveTransform(*t, phys->Velocity * deltaTime);
		phys->Force = {0, 0, 0};
	}
}
