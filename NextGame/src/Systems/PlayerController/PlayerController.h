#pragma once
#include "Components/Camera.h"
#include "Components/Mouse.h"
#include "Systems/System.h"
#include "Components/AABB.h"

/**
 * \brief Controls the Camera System + Player Input
 */
class PlayerController : System
{
public:
	explicit PlayerController(Scene& scene) : System(scene) {}

	void Init(Camera* c, Transform* t, Transform* p, Mouse* m)
	{
		Cam = c;
		CamTransform = t;
		PlayerTransform = p;
		Mouse = m;
	}

	void Update(float deltaTime) override;

	~PlayerController() override = default;
private:
	Camera* Cam{nullptr};
	Transform* CamTransform{nullptr};
	Transform* PlayerTransform{ nullptr };
	Mouse* Mouse{ nullptr };
};

