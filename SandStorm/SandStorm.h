#pragma once
#include <iostream>
#include <map>

#include "raylib.h"
#include "ElementRules.h"
#include "raymath.h"
#include "Cell.h"

class SandStorm 
{
public:
	SandStorm();

	void HandlePlacingCell();
	void Update(float deltaTime);
	void UpdateCell(int x, int y, Element::Elements element);

private:
	ElementRules* elementRules = nullptr;
	Texture2D cursor;
};