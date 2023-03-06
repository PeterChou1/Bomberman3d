#pragma once
#include "Components/Camera.h"
#include "Components/Map.h"
#include "Systems/System.h"

/**
 * \brief Controls the all bombs in the scene
 */
class BombManager : System
{
public:
	explicit BombManager(Scene& scene) : System(scene) {}
	void Update(float deltaTime) override;
	void Init(Map* m)
	{
		SceneMap = m;
	}

	~BombManager() override = default;
private:
	Map* SceneMap{ nullptr };
};

