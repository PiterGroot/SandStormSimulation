#include "SandStorm.h"

int UNOCCUPIED_CELL { 0 };

constexpr auto WIDTH = 512;
constexpr auto HEIGHT = 512;

int pixels[WIDTH * HEIGHT * 4];

Image screenImage;
Texture screenTexture;

SandStorm::SandStorm() //constructor
{
    cursor = LoadTexture("Textures/cursor.png");
    elementRules = new ElementRules();
    
    //screenImage = GenImageColor(WIDTH, HEIGHT, BLACK);
    screenImage.data = pixels;
    screenImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    screenImage.width = WIDTH;
    screenImage.height = HEIGHT;
    screenImage.mipmaps = 1;

    screenTexture = LoadTextureFromImage(screenImage);
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
        for (int y = 0; y < HEIGHT; y++)
        {
            //UpdateCell(x, y);
        }
    }
    UpdateTexture(screenTexture, pixels); //draw all updated cells

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexture(screenTexture, 0, 0, WHITE);
    
    //draw custom cursor
    Vector2 cursorPosition = Vector2((int)mousePosition.x - cursorOrigin, (int)mousePosition.y - cursorOrigin);
    DrawTexture(cursor, cursorPosition.x, cursorPosition.y, WHITE);
    
    DrawFPS(0, 0); //draw fps
    DrawText(GetElementString().c_str(), 0, 24, 24, GREEN); //draw current element and brush size
    EndDrawing();
}

//Update cell based on its rules
void SandStorm::UpdateCell(int x, int y)
{
    int index = (x + y * WIDTH) * 4;
    if (pixels[index + 3] == UNOCCUPIED_CELL)
        return;

    Element::Elements element = Element::SAND;
    //Element::Elements element = elementRules->cellColorValues[Color(index, (int)pixels[index + 1], (int)pixels[index + 2], 255)];

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

        int nextIndex = (x + xPos + y + yPos * WIDTH) * 4;
        if (pixels[nextIndex + 3] == UNOCCUPIED_CELL) //if next space is unoccupied 
        {
            Color cellColor = elementRules->GetCellColor(element);
            pixels[index + 3] = UNOCCUPIED_CELL;
            
            pixels[index] = cellColor.r;
            pixels[index + 1] = cellColor.g;
            pixels[index + 2] = cellColor.b;
            pixels[index + 3] = 255;
            break;
        }
    }
}

//Placing / destroying cells with mouse
void SandStorm::ManipulateCell(bool state, int xPos, int yPos)
{
    /*for (int x = -brushSize; x < brushSize; x++)
    {
        for (int y = -brushSize; y < brushSize; y++)
        {
            
        }
    }*/

    if (IsOutOfBounds(xPos, yPos)) //check for all brush positions if it is out of bounds
        return;

    int index = (xPos + yPos * WIDTH) * 4;
    if (state) //placing cells 
    {
        if (pixels[index + 3] != UNOCCUPIED_CELL) //check if chosen space is unoccupied
            return;

        pixels[index] = 255;
        pixels[index + 1] = 255;
        pixels[index + 2] = 255;
        pixels[index + 3] = 255;
    }
    else //removing cells
    {
        pixels[index] = 0;
        pixels[index + 1] = 0;
        pixels[index + 2] = 0;
        pixels[index + 3] = 0;
    }
}

//General input checks
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

//Switching between elements
void SandStorm::HandleCellSwitching()
{
    if (IsKeyPressed(KEY_ONE))
        currentElement = Element::Elements::SAND;

    if (IsKeyPressed(KEY_TWO))
        currentElement = Element::Elements::WATER;

    if (IsKeyPressed(KEY_THREE))
        currentElement = Element::Elements::WALL;

    if (IsKeyPressed(KEY_FOUR))
        currentElement = Element::Elements::TEST;
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
        case 4:  return "Test " +      std::to_string(brushSize);
        default: return "UNDIFINED " + std::to_string(brushSize);
    }
}