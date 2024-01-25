#include "ElementRules.h"

float aplhaRandomness = 200;

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
    sandRules =  { DOWN, DOWN_LEFT, DOWN_RIGHT                     };
    waterRules = { DOWN, RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN_RIGHT };
    smokeRules = { UP, UP_LEFT, UP_RIGHT, RIGHT, LEFT              };
    lavaRules =  { DOWN, RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN_RIGHT };

    // Initialize getRuleSet map
    getRuleSet = {
        { Element::Elements::SAND,   sandRules  },
        { Element::Elements::WATER,  waterRules },
        { Element::Elements::SMOKE,  smokeRules },
        { Element::Elements::LAVA,   lavaRules  }
    };

    // Initialize cell base color values
    cellColorValues = {
        { Element::Elements::SAND,     Color(255, 255, 0, 255)   },
        { Element::Elements::WATER,    Color(0, 0, 255, 255)     },
        { Element::Elements::WALL,     Color(255, 255, 255, 255) },
        { Element::Elements::SMOKE,    Color(150, 150, 150, 255) },
        { Element::Elements::LAVA,     Color(255, 77, 28, 255)   },
        { Element::Elements::OBSIDIAN, Color(255, 255, 255, 0)    }
    };
    
    // Initialize ruleValues map
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

//Returns a randomized color value based on input element
Color ElementRules::GetCellColor(Element::Elements element)
{
    Color baseColor = cellColorValues[element];

    int randAlpha = GetRandomValue(aplhaRandomness, 255); // randomize alpha
    return Color(baseColor.r, baseColor.g, baseColor.b, baseColor.a != 0 ? randAlpha : baseColor.a);
}