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
#include "Components/Display.h"
#include "Components/Map.h"
#include "Components/Mesh.h"
#include "Components/Mouse.h"
#include "Components/PlayerInfoObj.h"
#include "Components/Transform.h"
#include "Math/Vec3d.h"
#include "Systems/AI/AIController.h"
#include "Systems/Bomb/BombManager.h"
#include "Systems/CameraController/CameraController.h"
#include "Systems/GernadeHandler/GrenadeHandler.h"
#include "Systems/Mouse/MouseSystem.h"
#include "Systems/Physics2D/Physics2D.h"
#include "Systems/Physics3D/Physics3D.h"
#include "Systems/PlayerController/PlayerController.h"
#include "Systems/Renderer/Renderer.h"
#include "Systems/SoundSystem/SoundSystem.h"
#include "Systems/UI/UI.h"

int sComponentCounter = 0;
// global scene
Scene scene;
// object to keep track of all system
GlobalSystem* allSystem = new GlobalSystem();
// -- camera parameters ---
constexpr Vec3d LOOK_AT = { 0, 1.5, 0 };
constexpr Vec3d UP = { 0, 1, 0 };
constexpr Vec3d CAM_POS = { 0, 30, 15 };
// -- player parameters ---
constexpr Vec3d PLAYER_START = { 0, 1.5, 0 };
constexpr Vec3d PLAYER_SCALE = { 1, 3, 1 };
constexpr int BOMB_COOL_DOWN = 5;



//**
// * \brief Setup game map
// */
inline void SetupMap(Map& map, Scene& scene)
{
	for (int y = 0; y < MAP_WIDTH; y++)
	{
		for (int x = 0; x < MAP_HEIGHT; x++)
		{
			// Represents a wall
			if (LEVEL_1[y][x] == 1)
			{
				map.BlockMap[y][x] = 1;
				const EntityId meshId = scene.NewEntity();
				const auto mesh = scene.AddComponent<Mesh>(meshId);
				const auto meshTransform = scene.AddComponent<Transform>(meshId);
				const auto meshAABB = scene.AddComponent<AABB>(meshId);
				InitTransform(*meshTransform, {
								  static_cast<double>(-(MAP_HEIGHT / 2) + y), 0.5,
								  static_cast<double>(-(MAP_WIDTH / 2) + x)
					});
				LoadFromObjectFile("./TestData/unitcube.obj", *mesh);
				ComputeAABB(*mesh, *meshAABB);
			}
			// Represents a Agent AI
			else if (LEVEL_1[y][x] == 2)
			{
				const EntityId agentId = scene.NewEntity();
				const auto agentMesh = scene.AddComponent<Mesh>(agentId);
				const auto agentTransform = scene.AddComponent<Transform>(agentId);
				const auto agentAABB = scene.AddComponent<AABB>(agentId);
				scene.AddComponent<AI>(agentId);
				agentAABB->Stationary = false;
				Vec3d agentLocation = {
					static_cast<double>(-(MAP_HEIGHT / 2) + y), 1.5, static_cast<double>(-(MAP_WIDTH / 2) + x)
				};
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
	InitTransform(*playerTransform, PLAYER_START, {}, PLAYER_SCALE);
	LoadFromObjectFile("./TestData/unitcube.obj", *playerMesh);
	ComputeAABB(*playerMesh, *playerAABB);
	// ----- game config -----
	playerAABB->Stationary = false;
	playerAABB->IsPlayer = true;
	playerInfo->BombCooldown = BOMB_COOL_DOWN;
	// Uncomment for debug: playerInfo->Debug = true;

	// ----- Floor ----- (the floor is used exclusively to test collision with ground)
	const EntityId floorId = scene.NewEntity();
	const auto floorMesh = scene.AddComponent<Mesh>(floorId);
	const auto floorTransform = scene.AddComponent<Transform>(floorId);
	const auto floorAABB = scene.AddComponent<AABB>(floorId);
	InitTransform(*floorTransform, {0, -1, 0}, {}, {MAP_WIDTH, 1, MAP_HEIGHT });
	LoadFromObjectFile("./TestData/unitcube.obj", *floorMesh);
	ComputeAABB(*floorMesh, *floorAABB);
	floorAABB->Is3D = true;


	// ----- Camera -----
	const EntityId camId = scene.NewEntity();
	const auto camTransform = scene.AddComponent<Transform>(camId);
	const auto cam = scene.AddComponent<Camera>(camId);
	// Uncomment for debug: InitCamera(*cam, *camTransform, camPos, up, lookAt - camPos, fovAngle, nearPlane, farPlane, aspectRatio);
	AttachCamera(*cam, *camTransform, playerId, CAM_POS, UP, LOOK_AT - CAM_POS, FOV_ANGLE, NEAR_PLANE, FAR_PLANE, ASPECT_RATIO);

	// ----- Display -----
	const EntityId displayId = scene.NewEntity();
	const auto display = scene.AddComponent<Display>(displayId);
	display->Zbuffer = new double[RESOLUTION];
	display->AspectRatio = ASPECT_RATIO;
	display->Resolution = RESOLUTION;

	// ----- Map -----
	const EntityId mapId = scene.NewEntity();
	const auto mapComponent = scene.AddComponent<Map>(mapId);
	SetupMap(*mapComponent, scene);

	// ----- Init systems ------
	const auto playerControl = new PlayerController(scene, mapComponent, camTransform, playerTransform, playerInfo);
	const auto cameraControl = new CameraController(scene, cam, camTransform, mouse);
	const auto aiController = new AIController(scene, mapComponent, playerTransform);
	const auto renderer = new Renderer(scene, display, cam, camTransform);
	const auto phys3D = new Physics3D(scene);
	const auto phys2D = new Physics2D(scene);
	const auto mouseSystem = new MouseSystem(scene, mouse);
	const auto bombManager = new BombManager(scene, mapComponent);
	const auto grenadeHandler = new GrenadeHandler(scene, mapComponent, cam, camTransform, playerTransform, display, playerInfo, mouse);
	const auto uiSystem = new UI(scene, playerInfo);
	const auto soundSystem = new SoundSystem(scene);

	allSystem->RegisterSystem(playerControl);
	allSystem->RegisterSystem(cameraControl);
	allSystem->RegisterSystem(aiController);
	allSystem->RegisterSystem(renderer);
	allSystem->RegisterSystem(phys2D);
	allSystem->RegisterSystem(phys3D);
	allSystem->RegisterSystem(mouseSystem);
	allSystem->RegisterSystem(bombManager);
	allSystem->RegisterSystem(grenadeHandler);
	allSystem->RegisterSystem(uiSystem);
	allSystem->RegisterSystem(soundSystem);
}

//-------------------------------------- ----------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	// move camera (update loop)
	const float deltaSecond = deltaTime / 1000;
	allSystem->Update(deltaSecond);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	allSystem->Render();
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	delete allSystem;
}
