#include "CameraController.h"
#include <iostream>

CameraController::CameraController(Vector2 screenCenter)
{
	this->screenCenter = screenCenter;

	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	//cameraPosition = screenCenter;
	camera.offset = screenCenter;
}

void CameraController::OnUpdate(float deltaTime)
{
	camera.target = cameraPosition;
	camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

	if (camera.zoom > 10.0f) camera.zoom = 10.0f;
	else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

	if (IsKeyDown(KEY_W)) cameraPosition.y -= 100 * deltaTime;
	if (IsKeyDown(KEY_A)) cameraPosition.x -= 100 * deltaTime;
	if (IsKeyDown(KEY_S)) cameraPosition.y += 100 * deltaTime;
	if (IsKeyDown(KEY_D)) cameraPosition.x += 100 * deltaTime;
	std::cout << camera.zoom << "\n";
}
