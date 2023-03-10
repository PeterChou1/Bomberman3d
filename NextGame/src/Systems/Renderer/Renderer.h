#pragma once
#include "Components/Display.h"
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Systems/System.h"


/**
 * \brief Renderer deals with any entity with a Mesh and a Transform Attach to it
 */
class Renderer : public virtual System
{
public:
	Renderer(Scene& scene, Display* display, Camera* camera, Transform* camTransform) : 
		System(scene), Display(display), Cam(camera), CamTransform(camTransform) {}
	void Update(float deltaTime) override {}
	void Render() override;
	~Renderer() override
	{
		delete[] Display->Zbuffer;
	}
private:
	Display* Display{nullptr};
	Camera* Cam{nullptr};
	Transform* CamTransform{nullptr};
};