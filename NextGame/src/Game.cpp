//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
#define NOMINMAX
//------------------------------------------------------------------------
#include "App/main.h"
#include "app/app.h"
#include "ECS/Scene.h"
#include "Components/Camera.h"
#include "Components/Display.h"
#include "Components/Mesh.h"
#include "Components/PhysicsObj.h"
#include "Components/Transform.h"
#include "Math/Vec3d.h"
#include "Systems/Camera/CameraControl.h"
#include "Systems/Physics/PhysicsWorld.h"
#include "Systems/Renderer/Renderer.h"

// GLOBALS
int s_componentCounter = 0;
// --- scene ---
Scene scene;
// Systems
CameraControl cam_control = CameraControl(scene);
Renderer renderer = Renderer(scene);
PhysicsWorld phys = PhysicsWorld(scene);
// TODO: Camera transform used for debug purposes (REMOVE LATER)
Transform* c_transform;


//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// ----- parameters ------
	Vec3d look_at = { 0, 0, 0 };
	Vec3d up = { 0, 1, 0 };
	Vec3d cam_pos = { 0, 0, 35 };
	double FOV_angle = 90;
	double near_plane = 0.1;
	double far_plane = 100;
	double aspect_ratio = WINDOW_WIDTH / WINDOW_HEIGHT;
	int resolution = WINDOW_HEIGHT * WINDOW_WIDTH;
	
	scene = Scene();
	
	// ----- Init entities + components ------

	EntityID cam_id = scene.NewEntity();
	auto cam_transform = scene.AddComponent<Transform>(cam_id);
	auto cam = scene.AddComponent<Camera>(cam_id);

	c_transform = cam_transform;

	move_camera_transform(*cam_transform, cam_pos, up, look_at - cam_pos);
	init_camera(*cam, FOV_angle, near_plane, far_plane, aspect_ratio);
	
	EntityID display_id = scene.NewEntity();
	auto display = scene.AddComponent<Display>(display_id);
	display->zbuffer = new double[resolution];
	display->aspect_ratio = aspect_ratio;
	display->resolution = resolution;

	EntityID mesh_id = scene.NewEntity();
	auto mesh = scene.AddComponent<Mesh>(mesh_id);
	auto mesh_transform = scene.AddComponent<Transform>(mesh_id);
	auto phys = scene.AddComponent<PhysicsObj>(mesh_id);

	phys->force = { 10, 20, 0 };
	phys->velocity = { 5, 10, 0};
	phys->mass = 5;

	init_transform(*mesh_transform, { 0, 1, 0 });
	LoadFromObjectFile("./TestData/ship.obj", *mesh);

	EntityID cube_id = scene.NewEntity();
	auto cube_mesh = scene.AddComponent<Mesh>(cube_id);
	auto cube_transform = scene.AddComponent<Transform>(cube_id);
	init_transform(*cube_transform, { 0, -5, 0 }, {0, 0, 0}, {2, 1, 2});
	LoadFromObjectFile("./TestData/cubeflat.obj", *cube_mesh);

	// ----- Init systems ------
	cam_control.Init(cam, cam_transform);
	renderer.Init(display, cam, cam_transform);
}

//-------------------------------------- ----------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	// move camera (update loop)
	float deltaSecond = deltaTime / 1000;

	cam_control.Update(deltaSecond);
	phys.Update(deltaSecond);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	renderer.Render();
	char str[180];
	sprintf(str, "Camera Coords (%f, %f, %f)", c_transform->position.x, c_transform->position.y, c_transform->position.z);
	App::Print(100, 100, str);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}