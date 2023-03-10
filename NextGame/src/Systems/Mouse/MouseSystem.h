#pragma once
#include "Components/Mouse.h"
#include "Systems/System.h"

/**
 * \brief Handles Tracking of Mouse and Mouse Delta
 */
class MouseSystem : public virtual System
{
public:
	MouseSystem(Scene& scene, Mouse* mouse) : System(scene), Mouse(mouse) {}
	void Update(float deltaTime) override;
	void Render() override {}
	~MouseSystem() override = default;

private:
	Mouse* Mouse{ nullptr };
};