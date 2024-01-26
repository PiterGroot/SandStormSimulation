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

	typedef struct CellInfo {
		unsigned char type = 0;
		unsigned char updateTick = 0;
		unsigned char lifeTime = 0;
		bool isUpdated = false;
	};

	typedef struct AutoCellManipulator {
		bool mode;
		Vector3 position;
		Element::Elements placeElement;

		AutoCellManipulator() {};
		AutoCellManipulator(Vector2 position, int brushSize, bool mode, Element::Elements placeElement = Element::Elements::UNOCCUPIED)
		{
			this->mode = mode;
			this->position = Vector3(position.x, position.y, brushSize);
			this->placeElement = placeElement;
		}
	};

private:
	void UpdateCell(int x, int y);
	void ManipulateCell(bool state, int x, int y, Element::Elements placeElement, int overrideBrushSize = 0);
	
	void SetCell(int index, Element::Elements element, bool markUpdated = true);
	void SwapCell(int fromIndex, int toIndex, Element::Elements swapA, Element::Elements swapB);

	void HandleInput(int mouseX, int mouseY);
	void HandleCellSwitching();

	bool IsOutOfBounds(int x, int y);
	bool GetChance(float input);
	void ExportScreenShot();

	std::string GetElementString();
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
	bool skipTimerActive = false;
	bool showHudInfo = true;

	float cellPlacingNoRandomization = 0;
	float cellPlacingRandomization = 99;
	
	int brushSizeScaler = 5;
	int brushSize = 10;

	Sound removeAutoSFX;
	Sound resetSFX;
	Sound placeAutoSFX;
};