#pragma once
#include <raylib.h>

class CameraController
{
public:
	CameraController(Vector2 screenCenter);

	void OnUpdate(float deltaTime);
	Camera2D camera = { 0 };

private:
	Vector2 cameraPosition;
	Vector2 screenCenter;
};

