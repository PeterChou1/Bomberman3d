#pragma once
#include "Components/Camera.h"
#include "Components/Mouse.h"
#include "Systems/System.h"
#include "Components/AABB.h"
#include "Components/PlayerInfoObj.h"

constexpr double mouseSensitivity = 0.5;

/**
 * \brief Controls the Camera System
 */
class CameraController : public virtual System
{
public:
	explicit CameraController(Scene& scene, Camera* cam, Transform* camTransform, Mouse* mouse)
						     : System(scene), Cam(cam), CamTransform(camTransform), Mouse(mouse) {}
	void Update(float deltaTime) override;
	void Render() override {}
	~CameraController() override = default;
private:
	Camera* Cam{nullptr};
	Transform* CamTransform{nullptr};
	Mouse* Mouse{ nullptr };
};

