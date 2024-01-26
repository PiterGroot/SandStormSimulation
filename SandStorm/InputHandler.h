#pragma once
#include "SandStorm.h"

class InputHandler
{
public:
	InputHandler(SandStorm* engine, Vector2 screenCenter);
	void OnUpdate(Vector2 mousePosition);

private:
	SandStorm* engine = nullptr;
	Vector2 screenCenter = Vector2(0, 0);
};

