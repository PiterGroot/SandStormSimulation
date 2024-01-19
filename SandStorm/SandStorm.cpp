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
    Vector2 mousePosition = GetMousePosition();
   
    HandleCellSwitching();
    HandleInput((int)mousePosition.x, (int)mousePosition.y);

    // Update cell positions
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = HEIGHT - 2; y >= 0; y--) // Adjusted loop bounds
        {
            if (map[x][y] != 0)
                UpdateCell(cells[x][y].element, x, y);
        }
    }

    BeginDrawing();
    ClearBackground(BLACK);
  
    // Draw cells
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            if (map[x][y] != 0) 
                cells[x][y].Draw(x, y);
        }
    }
    
    DrawTexture(cursor, (int)mousePosition.x - 7, (int)mousePosition.y - 7, WHITE);
    DrawFPS(0, 0);

    DrawText(GetElementString().c_str(), 0, 24, 24, GREEN);
    EndDrawing();
}

//Update cell based on its rules
void SandStorm::UpdateCell(Element::Elements element, int x, int y)
{
    if (element == Element::Elements::WALL) //skip walls
        return;

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
            cells[x + xPos][y + yPos].cellColor = elementRules->GetCellColor(element);
            break;
        }
    }
}

//Input ceck for manipulating cells
void SandStorm::HandleInput(int mouseX, int mouseY)
{
    if (IsMouseButtonDown(0)) //placin cells
        ManipulateCell(true, mouseX, mouseY);

    if (IsMouseButtonDown(1)) //removing cells
        ManipulateCell(false, mouseX, mouseY);

    if (IsKeyPressed(KEY_LEFT_BRACKET)) //increasing brush size
        brushSize = abs(brushSize - 1);
        
    if (IsKeyPressed(KEY_RIGHT_BRACKET)) //decreasing brush size
        brushSize = abs(brushSize + 1);
}

//Placing / destroying cells with mouse
void SandStorm::ManipulateCell(bool state, int xPos, int yPos)
{
    for (int x = -brushSize; x < brushSize; x++)
    {
        for (int y = -brushSize; y < brushSize; y++)
        {
            Vector2 cellPosition = Vector2(x + xPos, y + yPos);
            if (IsOutOfBounds(cellPosition.x, cellPosition.y))
                continue;

            if (state) //Place cell 
            {
                if (map[(int)cellPosition.x][(int)cellPosition.y] != 0)
                    continue;
                
                map[(int)cellPosition.x][(int)cellPosition.y] = 1;
                cells[(int)cellPosition.x][(int)cellPosition.y].element = currentElement;
                cells[(int)cellPosition.x][(int)cellPosition.y].cellColor = elementRules->GetCellColor(currentElement);
            }
            else
            {
                map[(int)cellPosition.x][(int)cellPosition.y] = 0;
            }
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


//Checks if given position is outside the window
bool SandStorm::IsOutOfBounds(int posX, int posY)
{
    bool outOfBoundsA = posX > WIDTH || posY > HEIGHT;
    bool outOfBoundsB = posX < 0 || posY < 0;

    return outOfBoundsA || outOfBoundsB;
}

//Convert current element enum value to string for UI label
std::string SandStorm::GetElementString()
{
    switch (static_cast<int>(currentElement))
    {
        case 1:  return "Sand " + std::to_string(brushSize);
        case 2:  return "Water " + std::to_string(brushSize);
        case 3:  return "Wall " + std::to_string(brushSize);
        case 4:  return "Smoke " + std::to_string(brushSize);
        default: return "UNDIFINED " + std::to_string(brushSize);
    }
}