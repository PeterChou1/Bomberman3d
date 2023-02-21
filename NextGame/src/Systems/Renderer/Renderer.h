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
		display = d;
		cam = c;
		cam_transform = t;
	};

	void Render() override;

	~Renderer() override
	{
		delete[] display->zbuffer;
	}
private:
	Display* display;
	Camera* cam;
	Transform* cam_transform;
};