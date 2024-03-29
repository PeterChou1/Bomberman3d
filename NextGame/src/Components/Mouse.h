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

inline void GetMouseBoundary(bool& mouseLeft, bool& mouseRight, bool& mouseUp, bool& mouseDown)
{
	float tolerance = 10;
	float x, y;
	App::GetMousePos(x, y);
	mouseRight = x > APP_VIRTUAL_WIDTH - tolerance ? true : false;
	mouseLeft = x < 0 + tolerance ? true : false;
	mouseUp = y > APP_VIRTUAL_HEIGHT - tolerance ? true : false;
	mouseDown = y < 0 + tolerance ? true : false;
}


inline void InitMouse(Mouse& mouse, const float sensitivity, const float screenX, const float screenY)
{
	App::GetMousePos(mouse.MouseX, mouse.MouseY);
	mouse.DeltaY = mouse.DeltaX = 0;
	mouse.Sensitivity = sensitivity;
	mouse.ScreenX = screenX;
	mouse.ScreenY = screenY;
}