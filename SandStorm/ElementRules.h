#pragma once

#include "raylib.h"
#include "Element.h"
#include <unordered_map>
#include <vector>

class ElementRules
{
public:
	ElementRules();
	Color GetCellColor(Element::Elements element);
	
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

	std::unordered_map<Element::Elements, std::vector<Rules>> getRuleSet;
	std::unordered_map<Rules, Vector2> ruleValues;

private:
	float aplhaRandomness = 200;
	std::unordered_map<Element::Elements, Color> cellColorValues;

	std::vector<Rules> sandRules;
	std::vector<Rules> waterRules;
	std::vector<Rules> smokeRules;
};
