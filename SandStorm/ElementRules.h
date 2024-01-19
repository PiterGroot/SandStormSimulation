#pragma once

#include "raylib.h"
#include "raymath.h"
#include "Element.h"
#include <vector>
#include <map>

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

	std::map<Element::Elements, std::vector<Rules>> getRuleSet;
	std::map<Rules, Vector2> ruleValues;

private:
	int randomColorVariation = 50;
	std::map<Element::Elements, Color> cellColorValues;

	std::vector<Rules> sandRules;
	std::vector<Rules> waterRules;
	std::vector<Rules> wallRules;
	std::vector<Rules> smokeRules;
	std::vector<Rules> woodRules;
};
