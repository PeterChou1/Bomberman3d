#pragma once
#include "Components/Camera.h"
#include "Components/Mouse.h"
#include "Systems/System.h"
#include "Components/AABB.h"
#include "Components/PlayerInfoObj.h"

/**
 * \brief Controls the Camera System + Player Input
 */
class PlayerController : System
{
public:
	explicit PlayerController(Scene& scene) : System(scene) {}

	void Init(Camera* c, Transform* t, Transform* p, Mouse* m, PlayerInfoObj* pl)
	{
		Cam = c;
		CamTransform = t;
		PlayerTransform = p;
		Mouse = m;
		PlayerInfo = pl;
	}

	void Update(float deltaTime) override;

	~PlayerController() override = default;
private:
	Camera* Cam{nullptr};
	Transform* CamTransform{nullptr};
	Transform* PlayerTransform{ nullptr };
	PlayerInfoObj* PlayerInfo{ nullptr };
	Mouse* Mouse{ nullptr };
};

