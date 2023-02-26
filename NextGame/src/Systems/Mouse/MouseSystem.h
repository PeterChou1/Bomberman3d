#pragma once
#include "Components/Mouse.h"
#include "Systems/System.h"

class MouseSystem : System
{
public:
	MouseSystem(Scene& scene) : System(scene) {}

	void Init(Mouse* m)
	{
		_Mouse = m;
	}

	void Update(float deltaTime) override;

	~MouseSystem() override = default;

private:
	Mouse* _Mouse{ nullptr };
};