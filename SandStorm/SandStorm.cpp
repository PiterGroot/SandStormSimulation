#include "SandStorm.h"

constexpr auto WIDTH = 512;
constexpr auto HEIGHT = 512;

int map[WIDTH][HEIGHT];
Cell cells[WIDTH][HEIGHT];

SandStorm::SandStorm() 
{
    cursor = LoadTexture("Textures/cursor.png");
    elementRules = new ElementRules();
}

void SandStorm::Update(float deltaTime)
{
    BeginDrawing();
    ClearBackground(BLACK);

    Vector2 mousePosition = GetMousePosition();
    int mouseX = static_cast<int>(mousePosition.x);
    int mouseY = static_cast<int>(mousePosition.y);

    HandleCellSwitching();
    HandlePlacingCell(mouseX, mouseY);

    // Update cell positions
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = HEIGHT - 2; y >= 0; y--) // Adjusted loop bounds
        {
            if (map[x][y] != 0)
                UpdateCell(x, y, cells[x][y].element);
        }
    }

    // Draw cells
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            if (map[x][y] != 0) 
                cells[x][y].Draw(x, y);
        }
    }
    
    DrawTexture(cursor, mouseX - 7, mouseY - 7, WHITE);
    DrawFPS(0, 0);

    DrawText(GetElementString().c_str(), 0, 24, 24, GREEN);
    EndDrawing();
}

//Update cell based on its rules
void SandStorm::UpdateCell(int x, int y, Element::Elements element)
{
    auto cellRuleSet = elementRules->getRuleSet[element];
    for (const auto& rule : cellRuleSet)
    {
        Vector2 checkVector = elementRules->ruleValues[rule];
        int xPos = checkVector.x;
        int yPos = checkVector.y;

        if (map[x + xPos][y + yPos] == 0)
        {
            map[x][y] = 0;
            map[x + xPos][y + yPos] = 1;
            cells[x + xPos][y + yPos].element = element;
            cells[x + xPos][y + yPos].cellColor = elementRules->cellColorValues[element];
            break;
        }
    }
}

//TODO: automatically select element based on get key index
void SandStorm::HandleCellSwitching()
{
    if (IsKeyPressed(KEY_ONE))
        currentElement = Element::Elements::SAND;

    if (IsKeyPressed(KEY_TWO))
        currentElement = Element::Elements::WATER;

    if (IsKeyPressed(KEY_THREE))
        currentElement = Element::Elements::WALL;

    if (IsKeyPressed(KEY_FOUR))
        currentElement = Element::Elements::SMOKE;
}

std::string SandStorm::GetElementString() 
{
    switch (static_cast<int>(currentElement))
    {
        case 1:  return "Sand";
        case 2:  return "Water";
        case 3:  return "Wall";
        case 4:  return "Smoke";
        default: return "UNDIFINED";
    }
}

//Check for empty space to place cell
void SandStorm::HandlePlacingCell(int mouseX, int mouseY)
{
    bool outOfBoundsA = mouseX > WIDTH || mouseY > HEIGHT;
    bool outOfBoundsB = mouseX < 0 || mouseY < 0;

    if (outOfBoundsA || outOfBoundsB)
        return;

    if (IsMouseButtonDown(0)) 
    { 
        if (map[mouseX][mouseY] != 0)
            return;

        map[mouseX][mouseY] = 1;
        cells[mouseX][mouseY].element = currentElement;
        cells[mouseX][mouseY].cellColor = elementRules->cellColorValues[currentElement];
    }

    if (IsMouseButtonDown(1))
    {
        if (map[mouseX][mouseY] == 0)
            return;

        map[mouseX][mouseY] = 0;
    }
}