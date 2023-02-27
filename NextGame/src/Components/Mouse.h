#pragma once
#include "app/app.h"

struct Mouse
{
	float MouseX;
	float MouseY;
	// delta position based on previous mouse position
	float DeltaX;
	float DeltaY;
	float Sensitivity;
};


inline void InitMouse(Mouse& mouse, float Sensitivity)
{
	App::GetMousePos(mouse.MouseX, mouse.MouseY);
	mouse.DeltaY = mouse.DeltaX = 0;
	mouse.Sensitivity = Sensitivity;
}