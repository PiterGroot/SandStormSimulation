#pragma once
#include "Element.h"

class Cell
{
public:
	void Draw(int x, int y);
	Element::Elements element = Element::Elements::SAND;
};

