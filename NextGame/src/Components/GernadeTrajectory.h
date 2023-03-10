#pragma once
#include "Math/Vec3d.h"

/**
 * \brief Used for Rendering generade trajectory when the player press down E
 */
struct GernadeTrajectory
{
	Vec3d Start;
	Vec3d End;
	double FullPower = 5;
	double PowerGrowth = 2.5;
	double CurrentPower = 0;
};
