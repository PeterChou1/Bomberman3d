//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
#define NOMINMAX
//------------------------------------------------------------------------
#include "app/app.h"
#include "ECS/Scene.h"
#include "Components/Camera.h"
#include "Components/AABB.h"
#include "Components/AI.h"
#include "Components/Display.h"
#include "Components/Map.h"
#include "Components/Mesh.h"
#include "Components/Mouse.h"
#include "Components/PlayerInfoObj.h"
#include "Components/Transform.h"
#include "Math/Vec3d.h"
#include "Systems/AI/AIController.h"
#include "Systems/Bomb/BombManager.h"
#include "Systems/Mouse/MouseSystem.h"
#include "Systems/Physics2D/Physics2D.h"
#include "Systems/Physics3D/Physics3D.h"
#include "Systems/PlayerController/PlayerController.h"
#include "Systems/Renderer/Renderer.h"

int sComponentCounter = 0;
// --- scene ---
Scene scene;
// Systems
PlayerController playerControl = PlayerController(scene);
AIController aiController = AIController(scene);
Renderer renderer = Renderer(scene);
Physics3D phys3D = Physics3D(scene);
Physics2D phys2D = Physics2D(scene);
MouseSystem mouseSystem = MouseSystem(scene);
BombManager bombManager = BombManager(scene);
PlayerInfoObj* playerInfoObj;

void SetupMap(Map& map)
{
	for (int y = 0; y < MAP_WIDTH; y++)
	{
		for (int x = 0; x < MAP_HEIGHT; x++)
		{
			if (LEVEL_1[y][x] == 1)
			{
				map.BlockMap[y][x] = 1;
				const EntityId meshId = scene.NewEntity();
				const auto mesh = scene.AddComponent<Mesh>(meshId);
				const auto meshTransform = scene.AddComponent<Transform>(meshId);
				const auto meshAABB = scene.AddComponent<AABB>(meshId);
				InitTransform(*meshTransform, { static_cast<double>(-(MAP_HEIGHT / 2) + y), 0.5, static_cast<double>(-(MAP_WIDTH / 2) + x) });
				LoadFromObjectFile("./TestData/unitcube.obj", *mesh);
				ComputeAABB(*mesh, *meshAABB);
			}
			else if (LEVEL_1[y][x] == 2)
			{
				const EntityId agentId = scene.NewEntity();
				const auto agentMesh = scene.AddComponent<Mesh>(agentId);
				const auto agentTransform = scene.AddComponent<Transform>(agentId);
				const auto agentAABB = scene.AddComponent<AABB>(agentId);
				const auto agent = scene.AddComponent<AI>(agentId);
				agent->speed = 2;
				agentAABB->Stationary = false;
				Vec3d agentLocation = { static_cast<double>(-(MAP_HEIGHT / 2) + y), 1.5, static_cast<double>(-(MAP_WIDTH / 2) + x) };
				InitTransform(*agentTransform, agentLocation, {}, { 1, 3, 1 });
				LoadFromObjectFile("./TestData/unitcube.obj", *agentMesh);
				ComputeAABB(*agentMesh, *agentAABB);
			}
			else
			{
				map.BlockMap[y][x] = 0;
			}
		}
	}
}

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// ----- parameters ------
	constexpr Vec3d lookAt = { 1, 0, 0 };
	constexpr Vec3d up = { 0, 1, 0 };
	constexpr Vec3d playerStart = { 0, 1.5, 0 };
	constexpr Vec3d camPos = { 0, 30, 15 };

	constexpr double fovAngle = 90;
	constexpr double nearPlane = 0.1;
	constexpr double farPlane = 100;
	constexpr double aspectRatio = APP_VIRTUAL_WIDTH / APP_VIRTUAL_HEIGHT;
	constexpr int resolution = APP_VIRTUAL_HEIGHT * APP_VIRTUAL_WIDTH;

	scene = Scene();

	// ----- Init entities + components ------
	const EntityId mouseId = scene.NewEntity();
	const auto mouse = scene.AddComponent<Mouse>(mouseId);
	InitMouse(*mouse, 0.07, 0.01, -0.01);


	// ----- Player ----
	const EntityId playerId = scene.NewEntity();
	const auto playerInfo = scene.AddComponent<PlayerInfoObj>(playerId);
	const auto playerMesh = scene.AddComponent<Mesh>(playerId);
	const auto playerTransform = scene.AddComponent<Transform>(playerId);
	const auto playerAABB = scene.AddComponent<AABB>(playerId);

	InitTransform(*playerTransform, playerStart, {}, { 1, 3, 1 });
	LoadFromObjectFile("./TestData/unitcube.obj", *playerMesh);
	ComputeAABB(*playerMesh, *playerAABB);
	// ----- game config -----
	playerAABB->Stationary = false;
	playerAABB->isPlayer = true;
	playerInfo->BombCooldown = 5;

	// ----- Camera -----
	const EntityId camId = scene.NewEntity();
	const auto camTransform = scene.AddComponent<Transform>(camId);
	const auto cam = scene.AddComponent<Camera>(camId);
	AttachCamera(*cam, *camTransform, playerId, camPos, up, lookAt - camPos, fovAngle, nearPlane, farPlane, aspectRatio);


	// ----- Display ---
	const EntityId displayId = scene.NewEntity();
	const auto display = scene.AddComponent<Display>(displayId);
	display->Zbuffer = new double[resolution];
	display->AspectRatio = aspectRatio;
	display->Resolution = resolution;


	const EntityId mapId = scene.NewEntity();
	const auto mapComponent = scene.AddComponent<Map>(mapId);
	SetupMap(*mapComponent);

	// ----- Init systems ------
	playerControl.Init(cam, camTransform, playerTransform, mouse, playerInfo);
	renderer.Init(display, cam, camTransform);
	mouseSystem.Init(mouse);
	aiController.Init(mapComponent);
	bombManager.Init(mapComponent);
	playerInfoObj = playerInfo;
}

//-------------------------------------- ----------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	// move camera (update loop)
	const float deltaSecond = deltaTime / 1000;
	mouseSystem.Update(deltaSecond);
	playerControl.Update(deltaSecond);
	aiController.Update(deltaSecond);
	phys2D.Update(deltaSecond);
	phys3D.Update(deltaSecond);
	bombManager.Update(deltaSecond);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	renderer.Render();
	int count = 0;
	int aiCount[] = { GetId<AABB>(), GetId<Mesh>(), GetId<AI>() };
	const auto aiCountIterator = SceneIterator(scene, aiCount, 3);
	for (const EntityId entity : aiCountIterator)
	{
		count += 1;
	}
	// Basic UI Logic
	if (count == 0)
	{
		char YouWin[180];
		sprintf(YouWin, "You Win", playerInfoObj->CurrentCooldown);
		App::Print(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT / 2, YouWin);
	}
	else if (playerInfoObj->GameOver)
	{
		char GameOver[180];
		sprintf(GameOver, "Game Over", playerInfoObj->CurrentCooldown);
		App::Print(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT / 2, GameOver);
	}
	else
	{
		char BombCooldown[180];
		sprintf(BombCooldown, "Bomb cool down (%f)", playerInfoObj->CurrentCooldown);
		App::Print(100, 150, BombCooldown);
		char AICounter[180];
		sprintf(AICounter, "AI count (%d)", count);
		App::Print(100, 200, AICounter);
	}
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
