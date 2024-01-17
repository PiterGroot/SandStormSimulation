#pragma once
#include "SandStorm.h"

class Cell
{
	enum Element
	{
		Sand = 0
    };

public:
	Cell() {};
	Cell(int x, int y);

	void Update(SandStorm* engine);
	void Draw();

	Element element = Element::Sand;

private:
	bool active = true;
	Vector2 cellPosition;
};

