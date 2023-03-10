#pragma once
#include "Components/Camera.h"
#include "Components/Mouse.h"
#include "Systems/System.h"
#include "Components/AABB.h"
#include "Components/Display.h"
#include "Components/Map.h"
#include "Components/PlayerInfoObj.h"


/**
 * \brief Handles Player Throw Grenade Logic
 */
class GrenadeHandler : public virtual System
{
public:
	explicit GrenadeHandler(Scene& scene, Map* sceneMap, Camera* cam, Transform* camTransform,
		Transform* playerTransform, Display* display, PlayerInfoObj* playerInfoObj, Mouse* mouse)
		: System(scene), CamTransform(camTransform), Cam(cam), PlayerTransform(playerTransform),
		  PlayerInfo(playerInfoObj),
		  SceneMap(sceneMap), Display(display), Mouse(mouse)
	{
	}

	void Update(float deltaTime) override;

	void Render() override;

	~GrenadeHandler() override = default;

private:
	Transform* CamTransform{nullptr};
	Camera* Cam{nullptr};
	Transform* PlayerTransform{nullptr};
	PlayerInfoObj* PlayerInfo{nullptr};
	Map* SceneMap{nullptr};
	Display* Display{nullptr};
	Mouse* Mouse{ nullptr };
};
