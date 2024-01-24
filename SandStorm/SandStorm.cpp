#include "SandStorm.h"

Color UNOCCUPIED_CELL = Color(0,0,0,255);

constexpr auto WIDTH = 512;
constexpr auto HEIGHT = 512;
int size = WIDTH * HEIGHT;

std::vector<Color> simulation;
Color pixels[WIDTH * HEIGHT];

SandStorm::SandStorm() //constructor
{
    cursor = LoadTexture("Textures/cursor.png");

    screenImage = GenImageColor(WIDTH, HEIGHT, WHITE);
    screenTexture = LoadTextureFromImage(screenImage);

    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        pixels[i] = UNOCCUPIED_CELL;
    }

    int centerPosition = (WIDTH * .5f) + WIDTH * (HEIGHT * .5f);
    pixels[centerPosition] = GOLD;

    screenImage.data = pixels;
    elementRules = new ElementRules();
}

SandStorm::~SandStorm() //deconstructor
{
    delete elementRules;
}

void SandStorm::Update(float deltaTime)
{
    Vector2 mousePosition = GetMousePosition();

    simulation.clear();
    simulation.insert(simulation.end(), &pixels[0], &pixels[size]);

    HandleCellSwitching();
    HandleInput((int)mousePosition.x, (int)mousePosition.y);

    //Update all active cells
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT - 1; y++)
        {
            UpdateCell(x, y);
        }
    }

    UpdateTexture(screenTexture, pixels);   
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
    int oldIndex = x + WIDTH * y;
    if (CompareColor(simulation[oldIndex], UNOCCUPIED_CELL))
        return;

    Element::Elements element = Element::Elements::SAND;
    auto& cellRuleSet = elementRules->getRuleSet[element]; //get the right ruleset based on cell element type
    for (const auto& rule : cellRuleSet) //loop through all rules of the ruleset
    {
        Vector2 checkVector = elementRules->ruleValues[rule];
        int xPos = checkVector.x;
        int yPos = checkVector.y;

        int newIndex = (x + xPos) + WIDTH * (y + yPos);
        if (CompareColor(pixels[newIndex], UNOCCUPIED_CELL)) {
            pixels[oldIndex] = UNOCCUPIED_CELL;
            pixels[newIndex] = GOLD;
            break;
        }
    }
}

//Placing / destroying cells with mouse
void SandStorm::ManipulateCell(bool state, int xPos, int yPos)
{
    if (IsOutOfBounds(xPos, yPos)) //check for all brush positions if it is out of bounds
        return;

    int index = xPos + WIDTH * yPos;
    if (state) //placing cells 
    {
        if (CompareColor(pixels[index], UNOCCUPIED_CELL))
        {
            pixels[index] = GOLD;
        }
    }
    else
    {
        pixels[index] = UNOCCUPIED_CELL;
    }
}

//General input checks
void SandStorm::HandleInput(int mouseX, int mouseY)
{
    if (IsMouseButtonDown(0)) //placing cells
        ManipulateCell(true, mouseX, mouseY);

    if (IsMouseButtonDown(1)) //removing cells
        ManipulateCell(false, mouseX, mouseY);

    if (IsKeyPressed(KEY_LEFT_BRACKET)) //increase brush size
        brushSize = std::max(brushSize - 1, 1);

    if (IsKeyPressed(KEY_RIGHT_BRACKET)) //decrease brush size
        brushSize++;

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) //make screenshot
    {
        std::filesystem::path directoryPath = GetApplicationDirectory();
        directoryPath /= "Screenshots";

        if (!std::filesystem::exists(directoryPath))
        {
            std::filesystem::create_directory(directoryPath);
        }

        struct tm timeInfo;
        auto currentTime = std::chrono::system_clock::now();
        auto currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
        localtime_s(&timeInfo, &currentTimeT);

        strftime(timeBuffer, sizeof(timeBuffer), "%d-%H-%M-%S-", &timeInfo);

        std::string timeStamp(timeBuffer);
        std::filesystem::path imagePath = directoryPath / (timeStamp + "screenshot.png");

        Image image = LoadImageFromTexture(screenTexture);

        ExportImage(image, imagePath.string().c_str());
        UnloadImage(image);
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

    if (IsKeyPressed(KEY_FOUR))
        currentElement = Element::Elements::TEST;
}

//Simple color equals check
bool SandStorm::CompareColor(Color colorA, Color colorB)
{
    bool r = colorA.r == colorB.r;
    bool g = colorA.g == colorB.g;
    bool b = colorA.b == colorB.b;

    return r && g && b;
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