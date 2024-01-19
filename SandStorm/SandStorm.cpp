#include "SandStorm.h"

int UNOCCUPIED_CELL = 0;

constexpr auto WIDTH = 512;
constexpr auto HEIGHT = 512;

int map[WIDTH][HEIGHT];
Cell cells[WIDTH][HEIGHT];

SandStorm::SandStorm() //constructor
{
    cursor = LoadTexture("Textures/cursor.png");
    elementRules = new ElementRules();
}

SandStorm::~SandStorm() //deconstructor
{
    delete elementRules;
}

void SandStorm::Update(float deltaTime)
{
    Vector2 mousePosition = GetMousePosition();
   
    HandleCellSwitching();
    HandleInput((int)mousePosition.x, (int)mousePosition.y);

    //Update all active cells
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = HEIGHT - 2; y >= 0; y--)
        {
            if (map[x][y] != UNOCCUPIED_CELL)
                UpdateCell(cells[x][y].element, x, y);
        }
    }
  
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw all active cells
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            if (map[x][y] != UNOCCUPIED_CELL)
                cells[x][y].Draw(x, y);
        }
    }
    
    //draw custom cursor
    Vector2 cursorPosition = Vector2((int)mousePosition.x - cursorOrigin, (int)mousePosition.y - cursorOrigin);
    DrawTexture(cursor, cursorPosition.x, cursorPosition.y, WHITE);
    DrawFPS(0, 0); //draw fps
    DrawText(GetElementString().c_str(), 0, 24, 24, GREEN); //draw current element and brush size
    EndDrawing();
}

//Update cell based on its rules
void SandStorm::UpdateCell(Element::Elements element, int x, int y)
{
    if (element == Element::Elements::WALL) //skip walls
        return;

    auto cellRuleSet = elementRules->getRuleSet[element]; //get the right ruleset based on cell element type
    for (const auto& rule : cellRuleSet) //loop through all rules of the ruleset
    {
        Vector2 checkVector = elementRules->ruleValues[rule];
        int xPos = checkVector.x;
        int yPos = checkVector.y;

        if (IsOutOfBounds(x + xPos, y + yPos)) //check if next position would be out of bounds
            continue;

        if (map[x + xPos][y + yPos] == UNOCCUPIED_CELL) //if next space is unoccupied 
        {
            map[x][y] = UNOCCUPIED_CELL;
            map[x + xPos][y + yPos] = element;
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
        brushSize = std::max(brushSize - 1, 1);
        
    if (IsKeyPressed(KEY_RIGHT_BRACKET)) //decreasing brush size
        brushSize++;
}

//Placing / destroying cells with mouse
void SandStorm::ManipulateCell(bool state, int xPos, int yPos)
{
    for (int x = -brushSize; x < brushSize; x++)
    {
        for (int y = -brushSize; y < brushSize; y++)
        {
            Vector2 cellPosition = Vector2(x + xPos, y + yPos);
            if (IsOutOfBounds(cellPosition.x, cellPosition.y)) //check for all positions if it is out of bounds
                continue;

            if (state) //placing cells 
            {
                if (map[(int)cellPosition.x][(int)cellPosition.y] != UNOCCUPIED_CELL) //check if chosen space is unoccupied
                    continue;
                
                map[(int)cellPosition.x][(int)cellPosition.y] = currentElement;
                cells[(int)cellPosition.x][(int)cellPosition.y].element = currentElement;
                cells[(int)cellPosition.x][(int)cellPosition.y].cellColor = elementRules->GetCellColor(currentElement);
            }
            else //removing cells
            {
                map[(int)cellPosition.x][(int)cellPosition.y] = UNOCCUPIED_CELL;
            }
        }
    }
}

//Switching between elements
void SandStorm::HandleCellSwitching()
{
    if (IsKeyPressed(KEY_ONE))
        currentElement = Element::Elements::SAND;

    if (IsKeyPressed(KEY_TWO))
        currentElement = Element::Elements::WATER;

    if (IsKeyPressed(KEY_THREE))
        currentElement = Element::Elements::WALL;
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
        case 1:  return "Sand " +      std::to_string(brushSize);
        case 2:  return "Water " +     std::to_string(brushSize);
        case 3:  return "Wall " +      std::to_string(brushSize);
        default: return "UNDIFINED " + std::to_string(brushSize);
    }
}