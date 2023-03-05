#pragma once
#include "Systems/System.h"

/**
 * \brief Physics 3D
 */
class Physics2D : System
{
public:
	Physics2D(Scene& scene) : System(scene) {}

	void Update(float deltaTime) override;

	~Physics2D() override = default;
};