#pragma once
#include "ECS/Scene.h"


class System
{
public:
	System(Scene& scene) : SystemScene(scene) {}
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual ~System() = default;
protected:
	Scene& SystemScene;
};


class GlobalSystem
{
public:
	void RegisterSystem(System* sys)
	{
		SystemList.push_back(sys);
	}

	void Update(float deltaTime)
	{
		for (auto sys : SystemList)
		{
			sys->Update(deltaTime);
		}
	}

	void Render()
	{
		for (auto sys : SystemList)
		{
			sys->Render();
		}
	}
private:
	std::vector<System*> SystemList;
};