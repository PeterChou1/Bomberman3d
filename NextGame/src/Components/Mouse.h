#pragma once
#include "app/app.h"

struct Mouse
{
	float MouseX;
	float MouseY;
	// delta position based on previous mouse position
	float DeltaX;
	float DeltaY;
};


inline void InitMouse(Mouse& mouse)
{
	App::GetMousePos(mouse.MouseX, mouse.MouseY);
	mouse.DeltaY = mouse.DeltaX = 0;
}