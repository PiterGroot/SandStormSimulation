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
	void HandleInput(int mouseX, int mouseY);
	void UpdateCell(Element::Elements element, int x, int y);
	void HandleCellSwitching();
	void ManipulateCell(bool state, int x, int y);

	bool IsOutOfBounds(int x, int y);
	std::string GetElementString();

	Element::Elements currentElement = Element::Elements::SAND;
	int brushSize = 5;

	ElementRules* elementRules = nullptr;
	Texture2D cursor;
};