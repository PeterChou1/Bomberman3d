#pragma once
#include "Math/Vec3d.h"

struct RigidBody2D
{
	double Elasticity;
	double Mass;
	// we will only be restricted to x, z axis
	Vec3d Velocity;
	Vec3d Force;
};