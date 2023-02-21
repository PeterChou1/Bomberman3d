#pragma once
#include "ECS/Scene.h"


class System
{
public:
	System(Scene& scene) : m_scene(scene) {}
	virtual void Update(float deltaTime) {}
	virtual void Render() {}
	virtual ~System() {}
protected:
	Scene& m_scene;
};
