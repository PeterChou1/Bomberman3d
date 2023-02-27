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
#include "Components/Mouse.h"
#include "Components/Mouse.h"
#include "Components/Particle.h"
#include "Components/RigidBody.h"
#include "Components/Transform.h"
#include "Math/Vec3d.h"
#include "Systems/Camera/CameraControl.h"
#include "Systems/Mouse/MouseSystem.h"
#include "Systems/Physics/PhysicsWorld.h"
#include "Systems/Renderer/Renderer.h"

int sComponentCounter = 0;
// --- scene ---
Scene scene;
// Systems
CameraControl camControl = CameraControl(scene);
Renderer renderer = Renderer(scene);
PhysicsWorld phys = PhysicsWorld(scene);
MouseSystem mouseSystem = MouseSystem(scene);
// TODO: transform used for debug purposes (REMOVE LATER)
Transform* transform;
Transform* cameraTransform;
Mouse* _mouse;
Camera* _cam;
//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// ----- parameters ------
	constexpr Vec3d lookAt = { 1, 0, 0 };
	constexpr Vec3d up = { 0, 1, 0 };
	constexpr Vec3d camPos = { -7.45, 18.79, -12.35 };
	//Normalize(target);

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

	const EntityId camId = scene.NewEntity();
	const auto camTransform = scene.AddComponent<Transform>(camId);
	const auto cam = scene.AddComponent<Camera>(camId);
	//InitTransform(*camTransform, camPos);
	//InitCamera(*cam, fovAngle, nearPlane, farPlane, aspectRatio);
	InitCamera(*cam, *camTransform, camPos, up, lookAt - camPos, fovAngle, nearPlane, farPlane, aspectRatio);

	const EntityId displayId = scene.NewEntity();
	const auto display = scene.AddComponent<Display>(displayId);
	display->Zbuffer = new double[resolution];
	display->AspectRatio = aspectRatio;
	display->Resolution = resolution;




	const EntityId floorId = scene.NewEntity();
	const auto floorTransform = scene.AddComponent<Transform>(floorId);
	const auto floorMesh = scene.AddComponent<Mesh>(floorId);
	LoadFromObjectFile("./TestData/cubeflat.obj", *floorMesh);
	InitTransform(*floorTransform, { 0, 0, 0 });

	const EntityId meshId = scene.NewEntity();
	const auto mesh = scene.AddComponent<Mesh>(meshId);
	const auto meshTransform = scene.AddComponent<Transform>(meshId);
	InitTransform(*meshTransform, { 0, 5, 0 });
	LoadFromObjectFile("./TestData/unitcube.obj", *mesh);


	// TODO: debug remove
	transform = meshTransform;
	cameraTransform = camTransform;
	_mouse = mouse;
	_cam = cam;

	// ----- Init systems ------
	camControl.Init(cam, camTransform, mouse);
	renderer.Init(display, cam, camTransform);
	mouseSystem.Init(mouse);
}

//-------------------------------------- ----------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	// move camera (update loop)
	const float deltaSecond = deltaTime / 1000;

	// Rotate mesh for testing
	// RotateTransform(*transform, {0, 0, 0.1});
	mouseSystem.Update(deltaSecond);
	camControl.Update(deltaSecond);
	phys.Update(deltaSecond);
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
	char str6[180];
	//sprintf(str1, "(%f, %f, %f) (%f)\n", transform->Local2World[0][0], transform->Local2World[0][1], transform->Local2World[0][2], transform->Local2World[0][3]);
	//App::Print(100, 150, str1);
	sprintf(str2, "Up: (%f, %f, %f)", _cam->Up.X, _cam->Up.Y, _cam->Up.Z);
	App::Print(100, 50, str2);
	sprintf(str3, "Target: (%f, %f, %f)", _cam->Target.X, _cam->Target.Y, _cam->Target.Z);
	App::Print(100, 100, str3);
	sprintf(str4, "Cam Pos: (%f, %f, %f)", cameraTransform->Position.X, cameraTransform->Position.Y, cameraTransform->Position.Z);
	App::Print(100, 150, str4);
	float x, y;
	App::GetMousePos(x, y);
	sprintf(str6, "Mouse Cursor: (x:%f, y:%f)", x, y);
	App::Print(100, 200, str6);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
