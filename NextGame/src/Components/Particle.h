#pragma once
#include "Transform.h"
#include "Math/Vec3d.h"

/**
 * \brief Represents a particle in the physics system
 */
struct Particle
{
	Vec3d Velocity;
	Vec3d Force;
	double Mass;

	// whether or not particle is part of the spring mass system
	bool SpringMass;
	// Used configuration parameters for spring mass systems
	double RestLength;
	double SpringConstant;
	double DampingConstant;
	// The spring mass particle that this particle is connected to
	EntityId Partner;

	// whether or not the particle is affected by gravity
	bool Gravity;
};
