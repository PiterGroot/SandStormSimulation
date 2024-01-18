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
    int xMouse = static_cast<int>(mousePosition.x);
    int yMouse = static_cast<int>(mousePosition.y);

    HandlePlacingCell();

    // Update cell positions
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = HEIGHT - 2; y >= 0; y--) // Adjusted loop bounds
        {
            if (map[x][y] == 1)
                UpdateCell(x, y, cells[x][y].element);
        }
    }

    // Draw cells
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            if (map[x][y] == 1)
                cells[x][y].Draw(x, y);
        }
    }

    DrawTexture(cursor, xMouse - 7, yMouse - 7, WHITE);
    DrawFPS(0, 0);

    EndDrawing();
}

//Update cell based on its rules
void SandStorm::UpdateCell(int x, int y, Element::Elements element)
{
    if (element == Element::Elements::SAND) 
    {
        for (const auto& rule : elementRules->sandRules)
        {
            Vector2 checkVector = elementRules->ruleValues[rule];
            int xPos = checkVector.x;
            int yPos = checkVector.y;

            if (map[x + xPos][y + yPos] == 0) // Check if cell can move down
            {
                 map[x][y] = 0;
                 map[x + xPos][y + yPos] = 1;
                 break;
            }
        }
    }
}

//Check for empty space to place cell
void SandStorm::HandlePlacingCell()
{
    if (IsMouseButtonDown(0)) { //TODO: handle position outside window

        Vector2 mousePosition = GetMousePosition();

        bool outOfBoundsA = mousePosition.x > WIDTH || mousePosition.y > HEIGHT;
        bool outOfBoundsB = mousePosition.x < 0 || mousePosition.y < 0;

        if (outOfBoundsA || outOfBoundsB)
            return;

        int xPos = mousePosition.x;
        int yPos = mousePosition.y;

        if (map[xPos][yPos] == 1)
            return;

        map[xPos][yPos] = 1;
    }
}