#pragma once
#include <iostream>
#include <string>
#include <map>

#include "raylib.h"
#include "ElementRules.h"
#include "raymath.h"
#include "Cell.h"

class SandStorm 
{
public:
	SandStorm();
	void Update(float deltaTime);

private:
	void HandlePlacingCell(int mouseX, int mouseY);
	void UpdateCell(int x, int y, Element::Elements element);
	void HandleCellSwitching();
	std::string GetElementString();

	Element::Elements currentElement = Element::Elements::SAND;

	ElementRules* elementRules = nullptr;
	Texture2D cursor;
};