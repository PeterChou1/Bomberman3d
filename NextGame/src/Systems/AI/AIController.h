#pragma once
#include "Components/Map.h"
#include "Systems/System.h"

class AIController : public virtual System
{

public:
	explicit AIController(Scene& scene, Map* sceneMap, Transform* playerTransform) : System(scene), PlayerTransform(playerTransform), SceneMap(sceneMap) {}
	void Update(float deltaTime) override;
	void Render() override {}
	~AIController() override = default;
private:
	Map* SceneMap{ nullptr };
	Transform* PlayerTransform{ nullptr };
};

