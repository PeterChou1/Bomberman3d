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
#include "Components/Transform.h"
#include "Math/Vec3d.h"
#include "Systems/AI/AIController.h"
#include "Systems/Mouse/MouseSystem.h"
#include "Systems/Physics2D/Physics2D.h"
#include "Systems/Physics3D/Physics3D.h"
#include "Systems/PlayerController/PlayerController.h"
#include "Systems/Renderer/Renderer.h"

int sComponentCounter = 0;
// --- scene ---
Scene scene;
// Systems
PlayerController camControl = PlayerController(scene);
AIController aiController = AIController(scene);
Renderer renderer = Renderer(scene);
Physics3D phys3D = Physics3D(scene);
Physics2D phys2D = Physics2D(scene);
MouseSystem mouseSystem = MouseSystem(scene);


// TODO: transform used for debug purposes (REMOVE LATER)
Transform* transform;
Transform* cameraTransform;
Mouse* _mouse;
Camera* _cam;
AABB* _aabb;
AI* aidir;
Map* _map;

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
				aidir = agent;
				transform = agentTransform;
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
	const auto playerMesh = scene.AddComponent<Mesh>(playerId);
	const auto playerTransform = scene.AddComponent<Transform>(playerId);
	const auto playerAABB = scene.AddComponent<AABB>(playerId);
	InitTransform(*playerTransform, playerStart, {}, { 1, 3, 1 });
	LoadFromObjectFile("./TestData/unitcube.obj", *playerMesh);
	ComputeAABB(*playerMesh, *playerAABB);
	playerAABB->Stationary = false;

	const EntityId camId = scene.NewEntity();
	const auto camTransform = scene.AddComponent<Transform>(camId);
	const auto cam = scene.AddComponent<Camera>(camId);
	AttachCamera(*cam, *camTransform, playerId, camPos, up, lookAt - camPos, fovAngle, nearPlane, farPlane, aspectRatio);

	const EntityId displayId = scene.NewEntity();
	const auto display = scene.AddComponent<Display>(displayId);
	display->Zbuffer = new double[resolution];
	display->AspectRatio = aspectRatio;
	display->Resolution = resolution;


	const EntityId mapId = scene.NewEntity();
	const auto mapComponent = scene.AddComponent<Map>(mapId);
	SetupMap(*mapComponent);

	// TODO: debug remove
	cameraTransform = camTransform;
	//transform = playerTransform;
	_mouse = mouse;
	_cam = cam;
	_aabb = playerAABB;
	_map = mapComponent;

	// ----- Init systems ------
	camControl.Init(cam, camTransform, playerTransform, mouse);
	renderer.Init(display, cam, camTransform);
	mouseSystem.Init(mouse);
	aiController.Init(mapComponent);
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
	camControl.Update(deltaSecond);
	aiController.Update(deltaSecond);
	phys2D.Update(deltaSecond);
	phys3D.Update(deltaSecond);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	renderer.Render();
	char str2[180];
	char str3[180];
	char str4[180];
	//sprintf(str1, "(%f, %f, %f) (%f)\n", transform->Local2World[0][0], transform->Local2World[0][1], transform->Local2World[0][2], transform->Local2World[0][3]);
	//App::Print(100, 150, str1);
	sprintf(str2, "Up: (%f, %f, %f)", _cam->Up.X, _cam->Up.Y, _cam->Up.Z);
	App::Print(100, 50, str2);
	sprintf(str3, "Target: (%f, %f, %f)", _cam->Target.X, _cam->Target.Y, _cam->Target.Z);
	App::Print(100, 100, str3);
	//for (int y = 0; y < MAP_HEIGHT; y++)
	//{
	//	std::string ss = "Map: ";
	//	for (int x = 0; x < MAP_WIDTH; x++)
	//	{
	//		if (y == aidir->x && x == aidir->y)
	//		{
	//			ss += "[X]";
	//		} else if (_map->BlockMap[y][x] == 1)
	//		{
	//			ss += "[1]";
	//		} else if (_map->BlockMap[y][x] == 0)
	//		{
	//			ss += "[0]";
	//		}
	//	}
	//	App::Print(100, 200 + 20 * y, ss.c_str());
	//}
	char str6[180];
	sprintf(str6, "agent direction (x:%d, z:%d)", aidir->x, aidir->y);
	App::Print(100, 150, str6);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
