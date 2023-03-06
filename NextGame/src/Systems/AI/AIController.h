#pragma once
#include "Components/Map.h"
#include "Systems/System.h"

class AIController : System 
{

public:
	explicit AIController(Scene& scene) : System(scene) {}
	void Update(float deltaTime) override;
	void Init(Map* m)
	{
		SceneMap = m;
	}
	~AIController() override = default;
private:
	Map* SceneMap{ nullptr };
};

