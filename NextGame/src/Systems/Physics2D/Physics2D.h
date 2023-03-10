#pragma once
#include "Systems/System.h"

/**
 * \brief Physics 2D
 */
class Physics2D : public virtual System
{
public:
	Physics2D(Scene& scene) : System(scene) {}
	void Update(float deltaTime) override;
	void Render() override {}
	~Physics2D() override = default;
};