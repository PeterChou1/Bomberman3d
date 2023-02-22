#pragma once
#include "Components/Display.h"
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Systems/System.h"


/**
 * \brief Renderer deals with any entity with a Mesh and a Transform Attach to it
 */
class Renderer : System
{
public:
	Renderer(Scene& scene) : System(scene) {}

	void Init(Display* d, Camera* c, Transform* t)
	{
		Display = d;
		Cam = c;
		CamTransform = t;
	}

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