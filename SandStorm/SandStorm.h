#pragma once
#include <filesystem>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <map>

#include "raylib.h"
#include "ElementRules.h"
#include "raymath.h"

class SandStorm 
{
public:
	SandStorm();
	~SandStorm();

	void Update(float deltaTime);

private:
	void HandleInput(int mouseX, int mouseY);
	void UpdateCell(int x, int y);
	void HandleCellSwitching();
	void ManipulateCell(bool state, int x, int y);

	bool CompareColor(Color colorA, Color colorB);
	bool IsOutOfBounds(int x, int y);
	std::string GetElementString();

	ElementRules* elementRules = nullptr;
	Element::Elements currentElement = Element::Elements::SAND;

	Texture2D cursor;
	Texture2D screenTexture;
	Image screenImage;

	int cursorOrigin = 7;
	int brushSize = 5;
	char timeBuffer[20];
};