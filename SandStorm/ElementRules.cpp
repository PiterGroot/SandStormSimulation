#include "ElementRules.h"

ElementRules::ElementRules()
{
    // Initialize rule sets
    sandRules = { DOWN, DOWN_LEFT, DOWN_RIGHT };
    waterRules = { DOWN, DOWN_LEFT, DOWN_RIGHT, RIGHT, LEFT };

    // Initialize getRuleSet map
    getRuleSet = {
        { Element::Elements::SAND, sandRules },
        { Element::Elements::WATER, waterRules },
        { Element::Elements::WALL, wallRules },
    };

    // Initialize cellColorValues map
    cellColorValues = {
        { Element::Elements::SAND, Color(255, 255, 0, 255) },
        { Element::Elements::WATER, Color(0, 0, 255, 255) },
        { Element::Elements::WALL, Color(255, 255, 255, 255) },
    };

    // Initialize ruleValues map
    ruleValues = {
        { UP, Vector2(0, -1) },
        { RIGHT, Vector2(1, 0) },
        { DOWN, Vector2(0, 1) },
        { LEFT, Vector2(-1, 0) },
        { UP_RIGHT, Vector2(1, -1) },
        { UP_LEFT, Vector2(-1, -1) },
        { DOWN_RIGHT, Vector2(1, 1) },
        { DOWN_LEFT, Vector2(-1, 1) },
    };
}
