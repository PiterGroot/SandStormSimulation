#pragma once
#include <filesystem>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <map>

#include "raylib.h"
#include "ElementRules.h"

class SandStorm 
{
public:
	SandStorm();
	~SandStorm();

	void Update(float deltaTime);

private:
	void UpdateCell(int x, int y);
	void ManipulateCell(bool state, int x, int y);
	void HandleInput(int mouseX, int mouseY);
	void HandleCellSwitching();

	void ExportScreenShot();
	bool IsOutOfBounds(int x, int y);
	bool GetChance(float input);
	std::string GetElementString();

	ElementRules* elementRules = nullptr;
	Element::Elements currentElement = Element::Elements::SAND;

	Color UNOCCUPIED_CELL = Color(0, 0, 0, 255);

	Texture2D cursor;
	Texture2D screenTexture;
	Image screenImage;

	int cursorOrigin = 7;
	char timeBuffer[20];

	bool shouldUpdate = true;

	float skipTimer;
	float skipTime = .01f;
	bool skipTimerActive = false;

	float cellPlacingNoRandomization = 0;
	float cellPlacingRandomization = 99;
	
	int brushSizeScaler = 5;
	int brushSize = 5;

	Sound placeSFX;
	Sound place1SFX;
	Sound place2SFX;
};