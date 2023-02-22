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
#include "Components/ParticleObj.h"
#include "Components/Transform.h"
#include "Math/Vec3d.h"
#include "Systems/Camera/CameraControl.h"
#include "Systems/Physics/PhysicsWorld.h"
#include "Systems/Renderer/Renderer.h"

int sComponentCounter = 0;
// --- scene ---
Scene scene;
// Systems
CameraControl camControl = CameraControl(scene);
Renderer renderer = Renderer(scene);
PhysicsWorld phys = PhysicsWorld(scene);
// TODO: Camera transform used for debug purposes (REMOVE LATER)
Transform* cTransform;

void CreateMassSpring(Scene& scene)
{
	const EntityId particleA = scene.NewEntity();
	const auto particleAMesh = scene.AddComponent<Mesh>(particleA);
	const auto particleAPhys = scene.AddComponent<ParticleObj>(particleA);
	const auto particleATransform = scene.AddComponent<Transform>(particleA);

	InitTransform(*particleATransform, {0, 10, 0}, {0, 0, 0}, {0.5, 0.5, 0.5});
	LoadFromObjectFile("./TestData/icosphere.obj", *particleAMesh);
	particleAPhys->Mass = 1000;
	particleAPhys->Gravity = false;
	particleAPhys->SpringMass = true;
	particleAPhys->RestLength = 4;
	particleAPhys->DampingConstant = 1;
	particleAPhys->SpringConstant = 5;

	const EntityId particleB = scene.NewEntity();
	particleAPhys->Partner = particleB;
	const auto particleBMesh = scene.AddComponent<Mesh>(particleB);
	const auto particleBPhys = scene.AddComponent<ParticleObj>(particleB);
	const auto particleBTransform = scene.AddComponent<Transform>(particleB);

	InitTransform(*particleBTransform, {0, 5, 0}, {0, 0, 0}, {0.5, 0.5, 0.5});
	LoadFromObjectFile("./TestData/icosphere.obj", *particleBMesh);
	particleBPhys->Mass = 5;
	particleBPhys->Gravity = true;
	particleBPhys->SpringMass = false;
}

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// ----- parameters ------
	constexpr Vec3d lookAt = {0, 0, 0};
	constexpr Vec3d up = {0, 1, 0};
	constexpr Vec3d camPos = {0, 0, 35};
	constexpr double fovAngle = 90;
	constexpr double nearPlane = 0.1;
	constexpr double farPlane = 100;
	const double aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
	const int resolution = WINDOW_HEIGHT * WINDOW_WIDTH;

	scene = Scene();

	// ----- Init entities + components ------

	const EntityId camId = scene.NewEntity();
	const auto camTransform = scene.AddComponent<Transform>(camId);
	const auto cam = scene.AddComponent<Camera>(camId);

	cTransform = camTransform;

	SetTransform(*camTransform, camPos, up, lookAt - camPos);
	InitCamera(*cam, fovAngle, nearPlane, farPlane, aspectRatio);

	const EntityId displayId = scene.NewEntity();
	const auto display = scene.AddComponent<Display>(displayId);
	display->Zbuffer = new double[resolution];
	display->AspectRatio = aspectRatio;
	display->Resolution = resolution;

	CreateMassSpring(scene);

	//EntityID mesh_id = scene.NewEntity();
	//auto mesh = scene.AddComponent<Mesh>(mesh_id);
	//auto mesh_transform = scene.AddComponent<Transform>(mesh_id);
	//auto phys = scene.AddComponent<ParticleObj>(mesh_id);
	//phys->force = { 10, 20, 0 };
	//phys->velocity = { 5, 10, 0};
	//phys->mass = 5;
	//init_transform(*mesh_transform, { 0, 1, 0 });
	//LoadFromObjectFile("./TestData/ship.obj", *mesh);

	const EntityId cubeId = scene.NewEntity();
	const auto cubeMesh = scene.AddComponent<Mesh>(cubeId);
	const auto cubeTransform = scene.AddComponent<Transform>(cubeId);
	InitTransform(*cubeTransform, {0, -5, 0}, {0, 0, 0}, {2, 1, 2});
	LoadFromObjectFile("./TestData/cubeflat.obj", *cubeMesh);

	// ----- Init systems ------
	camControl.Init(cam, camTransform);
	renderer.Init(display, cam, camTransform);
}

//-------------------------------------- ----------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	// move camera (update loop)
	const float deltaSecond = deltaTime / 1000;
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
	char str[180];
	sprintf(str, "Camera Coords (%f, %f, %f)", cTransform->Position.X, cTransform->Position.Y,
	        cTransform->Position.Z);
	App::Print(100, 100, str);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
