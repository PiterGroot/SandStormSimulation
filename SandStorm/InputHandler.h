#pragma once
#include "raylib.h"

class InputHandler
{
public:
	InputHandler(Vector2 screenCenter);
	void OnUpdate(Vector2 mousePosition);

private:
	void OnResetSim();
	Vector2 screenCenter = Vector2(0, 0);
};

