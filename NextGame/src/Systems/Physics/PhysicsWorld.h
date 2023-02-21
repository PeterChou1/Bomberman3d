#pragma once
#include "Systems/System.h"

/**
 * \brief Physics
 */
class PhysicsWorld : System
{
public:
	PhysicsWorld(Scene& scene) : System(scene) {}

	void Update(float deltaTime) override;

	~PhysicsWorld() override = default;
};

