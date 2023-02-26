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
#include "Components/RigidBody.h"
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
// TODO: transform used for debug purposes (REMOVE LATER)
Transform* transform;
Transform* cameraTransform;


//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// ----- parameters ------
	constexpr Vec3d lookAt = {0, 0, 0};
	constexpr Vec3d up = {0, 1, 0};
	constexpr Vec3d camPos = {0, 15, 15};
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
	SetTransform(*camTransform, camPos, up, lookAt - camPos);
	InitCamera(*cam, fovAngle, nearPlane, farPlane, aspectRatio);

	const EntityId displayId = scene.NewEntity();
	const auto display = scene.AddComponent<Display>(displayId);
	display->Zbuffer = new double[resolution];
	display->AspectRatio = aspectRatio;
	display->Resolution = resolution;

	// const EntityId cubeId = scene.NewEntity();
	// const auto cubeMesh = scene.AddComponent<Mesh>(cubeId);
	// cubeTransform = scene.AddComponent<Transform>(cubeId);
	// const auto rigidBody = scene.AddComponent<RigidBody>(cubeId);
	// InitTransform(*cubeTransform, {0, 5, 0});
	// LoadFromObjectFile("./TestData/unitcube.obj", *cubeMesh);
	// SetUpRigidBody(*rigidBody, *cubeTransform);


	const EntityId meshId = scene.NewEntity();
	const auto mesh = scene.AddComponent<Mesh>(meshId);
	const auto meshTransform = scene.AddComponent<Transform>(meshId);
	InitTransform(*meshTransform, { 0, 5, 0 });
	LoadFromObjectFile("./TestData/unitcube.obj", *mesh);

	transform = meshTransform;
	cameraTransform = camTransform;

	const EntityId floorId = scene.NewEntity();
	const auto floorTransform = scene.AddComponent<Transform>(floorId);
	const auto floorMesh = scene.AddComponent<Mesh>(floorId);
	LoadFromObjectFile("./TestData/cubeflat.obj", *floorMesh);
	InitTransform(*floorTransform, { 0, 0, 0 });

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

	// Rotate mesh for testing
	RotateTransform(*transform, {0.1, 0.1, 0});

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


	char str1[180];
	char str2[180];
	char str3[180];
	char str4[180];
	char str5[180];

	//sprintf(str1, "(%f, %f, %f) (%f)\n", transform->Local2World[0][0], transform->Local2World[0][1], transform->Local2World[0][2], transform->Local2World[0][3]);
	//App::Print(100, 150, str1);
	//sprintf(str2, "(%f, %f, %f) (%f)\n", transform->Local2World[1][0], transform->Local2World[1][1], transform->Local2World[1][2], transform->Local2World[1][3]);
	//App::Print(100, 100, str2);
	//sprintf(str3, "(%f, %f, %f) (%f)\n", transform->Local2World[2][0], transform->Local2World[2][1], transform->Local2World[2][2], transform->Local2World[2][3]);
	//App::Print(100, 50, str3);
	Vec3d r;
	GetEulerAngles(cameraTransform->Local2World, r);
	sprintf(str4, "Euler Angles (X: %f, Y: %f, Z: %f) rotation[2][0] (%f)", r.X,  r.Y, r.Z, cameraTransform->Local2World[2][0]);
	App::Print(100, 200, str4);
	float MouseX, MouseY;
	App::GetMouseAxis(MouseX, MouseY);
	sprintf(str5, "Mouse: (%f %f)", MouseX, MouseY);
	App::Print(100, 250, str5);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
