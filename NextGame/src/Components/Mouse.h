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
	// preconfigure delta for if the mouse is on the edge of the screen
	float ScreenX;
	float ScreenY;
};


inline void InitMouse(Mouse& mouse, const float sensitivity, const float screenX, const float screenY)
{
	App::GetMousePos(mouse.MouseX, mouse.MouseY);
	mouse.DeltaY = mouse.DeltaX = 0;
	mouse.Sensitivity = sensitivity;
	mouse.ScreenX = screenX;
	mouse.ScreenY = screenY;
}