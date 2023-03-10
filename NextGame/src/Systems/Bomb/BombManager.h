#pragma once
#include "Components/Camera.h"
#include "Components/Map.h"
#include "Systems/System.h"


constexpr float bombExplosionColorR = 1;
constexpr float bombExplosionColorG = 0;
constexpr float bombExplosionColorB = 0;

/**
 * \brief Controls the all bombs in the scene
 */
class BombManager : public virtual System
{
public:
	explicit BombManager(Scene& scene, Map* sceneMap) : System(scene), SceneMap(sceneMap) {}
	void Update(float deltaTime) override;
	void Render() override {}
	~BombManager() override = default;
private:
	Map* SceneMap{ nullptr };
};

