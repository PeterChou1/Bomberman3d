#include "stdafx.h"
#include "MouseSystem.h"
#include "app/app.h"


void MouseSystem::Update(float deltaTime)
{
	float newMouseX, newMouseY;
	App::GetMousePos(newMouseX, newMouseY);
	_Mouse->DeltaX = -(newMouseX - _Mouse->MouseX) * deltaTime * _Mouse->Sensitivity;
	_Mouse->DeltaY = -(newMouseY - _Mouse->MouseY) * deltaTime * _Mouse->Sensitivity;
	_Mouse->MouseX = newMouseX;
	_Mouse->MouseY = newMouseY;
}
