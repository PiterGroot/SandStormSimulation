#pragma once

#include "raylib.h"
#include <vector>
#include <map>

class ElementRules
{
public:
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
	
	ElementRules();
	std::vector<Rules> sandRules;
	std::map<Rules, Vector2> ruleValues;
};
