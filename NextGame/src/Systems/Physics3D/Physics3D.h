#pragma once
#include "Systems/System.h"

/**
 * \brief Physics 3D 
 */
class Physics3D : public virtual System
{
public:
	Physics3D(Scene& scene) : System(scene) {}
	void Update(float deltaTime) override;
	void Render() override {}
	~Physics3D() override = default;
};

