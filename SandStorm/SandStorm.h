#pragma once
#include <filesystem>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#include "raylib.h"
#include "ElementRules.h"
#include "InputHandler.h"
#include "ImageImporter.h"
#include "CameraController.h"

class SandStorm 
{
public:
	SandStorm();
	~SandStorm();

	void Update(float deltaTime);
	void Render();

	void SetCell(int index, Element::Elements element, bool markUpdated = true);
	void ManipulateCell(bool state, int x, int y, Element::Elements placeElement, int overrideBrushSize = 0);

	void ResetSim();
	void ExportScreenShot();

	int brushSize = 10;
	int brushSizeScaler = 5;
	
	static SandStorm* instance;
	Element::Elements currentElement = Element::Elements::SAND;
		
	Sound removeAutoSFX;
	Sound resetSFX;
	Sound placeAutoSFX;

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
	std::vector<AutoCellManipulator> autoManipulators;
	ImageImporter* imageImporter = nullptr;
	
	bool shouldUpdate = true;
	bool skipTimerActive = false;
	bool showHudInfo = true;

private:
	void UpdateCell(int x, int y);
	
	void SwapCell(int fromIndex, int toIndex, Element::Elements swapA, Element::Elements swapB);

	void HandleCellSwitching();

	bool IsOutOfBounds(int x, int y);
	bool GetChance(float input);

	std::string GetElementString();

	InputHandler* inputHandler = nullptr;
	ElementRules* elementRules = nullptr;
	CameraController* cameraController = nullptr;

	Color UNOCCUPIED_CELL = Color(0, 0, 0, 255);
	
	Texture2D cursor;
	Texture2D screenTexture;
	Image screenImage;

	Vector2 mousePosition;
	int cursorOrigin = 7;
	char timeBuffer[20];

	float cellPlacingNoRandomization = 0;
	float cellPlacingRandomization = 99;
};