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
    sandRules =  { DOWN, DOWN_LEFT, DOWN_RIGHT };
    waterRules = { DOWN, RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN_RIGHT };
    testRules =  { UP };

    // Initialize getRuleSet map
    getRuleSet = {
        { Element::Elements::SAND,  sandRules },
        { Element::Elements::WATER, waterRules },
        { Element::Elements::WALL,  wallRules },
        { Element::Elements::TEST,  testRules },
    };

    //// Initialize ruleValues map
    ruleValues = {
        { UP,         Vector2(0, -1) },
        { RIGHT,      Vector2(1, 0)  },
        { DOWN,       Vector2(0, 1)  },
        { LEFT,       Vector2(-1, 0) },
        { UP_RIGHT,   Vector2(1, -1) },
        { UP_LEFT,    Vector2(-1, -1)},
        { DOWN_RIGHT, Vector2(1, 1)  },
        { DOWN_LEFT,  Vector2(-1, 1) },
    };
}

Color ElementRules::GetCellColor(Element::Elements element)
{
    //for (const auto& labelPair : cellColorValues)
    //{
    //    if (element == labelPair.second) 
    //    {
    //        //int randAlpha = GetRandomValue(200, 255); // randomize alpha
    //        return Color(labelPair.first.r, labelPair.first.g, labelPair.first.b, 255);
    //    }
    //}
    
    //return PURPLE; //element not found color

    return RAYWHITE;
}