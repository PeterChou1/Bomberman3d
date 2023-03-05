#pragma once
#include "Systems/System.h"

/**
 * \brief Physics 3D 
 */
class Physics3D : System
{
public:
	Physics3D(Scene& scene) : System(scene) {}

	void Update(float deltaTime) override;

	~Physics3D() override = default;
};

