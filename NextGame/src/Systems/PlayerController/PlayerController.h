#pragma once
#include "Components/Mouse.h"
#include "Systems/System.h"
#include "Components/AABB.h"
#include "Components/Map.h"
#include "Components/PlayerInfoObj.h"

constexpr double playerSpeed = 5;
constexpr float bombColorR = 0.5;
constexpr float bombColorG = 0.5;
constexpr float bombColorB = 0.5;
constexpr float bombY = 0.5;

/**
 * \brief Handles Player Input
 */
class PlayerController : public virtual System
{
public:
	explicit PlayerController(Scene& scene, Map* sceneMap, Transform* camTransform, Transform* playerTransform, PlayerInfoObj* playerInfoObj)
						     : System(scene), CamTransform(camTransform), PlayerTransform(playerTransform),
						       PlayerInfo(playerInfoObj), SceneMap(sceneMap) {}
	void Update(float deltaTime) override;
	void Render() override {}
	~PlayerController() override = default;
private:
	Transform* CamTransform{nullptr};
	Transform* PlayerTransform{ nullptr };
	PlayerInfoObj* PlayerInfo{ nullptr };
	Map* SceneMap{ nullptr };
};

