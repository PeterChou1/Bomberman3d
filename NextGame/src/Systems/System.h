#pragma once
#include "ECS/Scene.h"


class System
{
public:
	System(Scene& scene) : SystemScene(scene) {}
	virtual void Update(float deltaTime) {}
	virtual void Render() {}
	virtual ~System() = default;
protected:
	Scene& SystemScene;
};
