#pragma once

#include "raylib.h"
#include "Element.h"
#include <vector>
#include <map>

class ElementRules
{
public:
	ElementRules();
	std::map<Element::Elements, Color> cellColorValues;

	Color sandColor = Color(255, 255, 0, 255);
	Color waterColor = Color(0, 0, 255, 255);
	Color wallColor = Color(255, 255, 255, 255);
	
	enum Rules
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		UP_RIGHT,
		UP_LEFT,
		DOWN_RIGHT,
		DOWN_LEFT,
	};

	std::map<Rules, Vector2> ruleValues;
	std::vector<Rules> sandRules;
};
