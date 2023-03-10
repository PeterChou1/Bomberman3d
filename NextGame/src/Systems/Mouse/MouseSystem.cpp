#include "stdafx.h"
#include "MouseSystem.h"
#include "app/app.h"


void MouseSystem::Update(const float deltaTime)
{
	float newMouseX, newMouseY;
	App::GetMousePos(newMouseX, newMouseY);
	Mouse->DeltaX = -(newMouseX - Mouse->MouseX) * deltaTime * Mouse->Sensitivity;
	Mouse->DeltaY = -(newMouseY - Mouse->MouseY) * deltaTime * Mouse->Sensitivity;
	Mouse->MouseX = newMouseX;
	Mouse->MouseY = newMouseY;
}
