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
	std::vector<Rules> sandRules;
	std::vector<Rules> waterRules;
	std::vector<Rules> wallRules;
	std::vector<Rules> smokeRules;
};
