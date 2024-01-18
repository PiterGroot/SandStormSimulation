#include "ElementRules.h"

ElementRules::ElementRules()
{
	cellColorValues =
	{
		{ Element::Elements::SAND,  sandColor},
		{ Element::Elements::WATER, waterColor},
		{ Element::Elements::WALL,  wallColor},
	};
	
	ruleValues = 
	{
		{ UP,         Vector2(0, -1)},
		{ RIGHT,      Vector2(1, 0)},
		{ DOWN,       Vector2(0, 1)},
		{ LEFT,       Vector2(-1, 0)},
		{ UP_RIGHT,   Vector2(1, -1)},
		{ UP_LEFT,    Vector2(-1, -1)},
		{ DOWN_RIGHT, Vector2(1, 1)},
		{ DOWN_LEFT,  Vector2(-1, 1)},
	};

	sandRules = 
	{
		DOWN,
		DOWN_LEFT,
		DOWN_RIGHT
	};
}
