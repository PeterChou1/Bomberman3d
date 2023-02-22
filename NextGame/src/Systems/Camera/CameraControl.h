#pragma once
#include "Components/Camera.h"
#include "Systems/System.h"

/**
 * \brief Camera System handle controls of a Camera
 */
class CameraControl : System
{
public:

	CameraControl(Scene& scene) : System(scene) {}


	void Init(Camera* c, Transform* t)
	{
		Cam = c;
		CamTransform = t;
	}

	void Update(float deltaTime) override;

	~CameraControl() override = default;
private:
	Camera* Cam{nullptr};
	Transform* CamTransform{nullptr};
};

