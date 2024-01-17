#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <map>

class SandStorm 
{
public:
	SandStorm();

	void HandlePlacingCell();
	void Update(float deltaTime);
};