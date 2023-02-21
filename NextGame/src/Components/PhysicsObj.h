#pragma once
#include "Math/Vec3d.h"

struct PhysicsObj
{
	Vec3d velocity;
	Vec3d force;
	double mass;
};