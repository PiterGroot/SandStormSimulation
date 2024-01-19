#include "ElementRules.h"

ElementRules::ElementRules()
{
    //Adding new cells steps:
    //  1. Add a new element type (Elements::Element)
    //  2. Add a hotkey check for switching to new cell
    //  3. Add string -> enum conversion for UI label
    //  4. Add rules for new cell
    //  5. Add color for new cell
    //  6. Bind element to its ruleset

    // Initialize rule sets
    sandRules = { DOWN, DOWN_LEFT, DOWN_RIGHT };
    waterRules = { DOWN, DOWN_LEFT, DOWN_RIGHT, RIGHT, LEFT };
    smokeRules = { UP, UP_LEFT, UP_RIGHT }; //TODO: UP rule buggy

    // Initialize getRuleSet map
    getRuleSet = {
        { Element::Elements::SAND, sandRules },
        { Element::Elements::WATER, waterRules },
        { Element::Elements::WALL, wallRules },
        { Element::Elements::SMOKE, smokeRules },
    };

    // Initialize cellColorValues map
    cellColorValues = {
        { Element::Elements::SAND, Color(255, 255, 0, 255) },
        { Element::Elements::WATER, Color(0, 0, 255, 255) },
        { Element::Elements::WALL, Color(255, 255, 255, 255) },
        { Element::Elements::SMOKE, Color(133, 133, 133, 255) }
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

Color ElementRules::GetCellColor(Element::Elements element)
{
    Color baseColor = cellColorValues[element];

    // adjust the RGB values randomly
    int r = Clamp(baseColor.r + GetRandomValue(-randomColorVariation, randomColorVariation), 0, 255);
    int g = Clamp(baseColor.g + GetRandomValue(-randomColorVariation, randomColorVariation), 0, 255);
    int b = Clamp(baseColor.b + GetRandomValue(-randomColorVariation, randomColorVariation), 0, 255);

    return Color(r, g, b, baseColor.a);
}