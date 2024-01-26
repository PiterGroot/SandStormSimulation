#pragma once
#include <filesystem>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#include "raylib.h"
#include "ElementRules.h"

class SandStorm 
{
public:
	SandStorm();
	~SandStorm();

	void Update(float deltaTime);
	void Render();

private:
	void UpdateCell(int x, int y);
	void ManipulateCell(bool state, int x, int y, int overrideBrushSize = 0);
	
	void SetCell(int index, Element::Elements element, bool markUpdated = true);
	void SwapCell(int fromIndex, int toIndex, Element::Elements swapA, Element::Elements swapB);

	void HandleInput(int mouseX, int mouseY);
	void HandleCellSwitching();

	bool IsOutOfBounds(int x, int y);
	bool GetChance(float input);
	void ExportScreenShot();

	std::string GetElementString();

	typedef struct AutoCellManipulator {

		Vector3 position;
		bool mode;

		AutoCellManipulator() {};
		AutoCellManipulator(Vector2 position, int brushSize, bool mode)
		{
			this->position = Vector3(position.x, position.y, brushSize);
			this->mode = mode;
		}
	};

	std::vector<AutoCellManipulator> autoManipulators;

	ElementRules* elementRules = nullptr;
	Element::Elements currentElement = Element::Elements::SAND;

	Color UNOCCUPIED_CELL = Color(0, 0, 0, 255);
	
	Texture2D cursor;
	Texture2D screenTexture;
	Image screenImage;

	Vector2 mousePosition;
	int cursorOrigin = 7;
	char timeBuffer[20];

	bool shouldUpdate = true;

	float skipTimer;
	float skipTime = .01f;
	bool skipTimerActive = false;

	float cellPlacingNoRandomization = 0;
	float cellPlacingRandomization = 99;
	
	int brushSizeScaler = 5;
	int brushSize = 10;

	Sound placeSFX;
	Sound place1SFX;
	Sound place2SFX;
};