#pragma once
#include "raylib.h"
#include "Element.h"

class Cell
{
public:
	void Draw(int x, int y);
	Element::Elements element = Element::Elements::SAND;
	Color cellColor = Color(255, 255, 0, 255);
};

