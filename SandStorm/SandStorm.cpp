#include "SandStorm.h"

constexpr auto WIDTH = 512;
constexpr auto HEIGHT = 512;

int size = WIDTH * HEIGHT;
Vector2 screenCenter = Vector2(WIDTH / 2, HEIGHT / 2);

Color pixels[WIDTH * HEIGHT];
SandStorm::CellInfo map[WIDTH * HEIGHT];

SandStorm::SandStorm() //constructor
{
    cursor = LoadTexture("Textures/cursor.png");

    screenImage = GenImageColor(WIDTH, HEIGHT, UNOCCUPIED_CELL);
    screenTexture = LoadTextureFromImage(screenImage);

    for (int i = 0; i < WIDTH * HEIGHT; i++) //set texture background to black
    {
        pixels[i] = UNOCCUPIED_CELL;
    }

    screenImage.data = pixels; //update image with black background
    elementRules = new ElementRules(); //create cell rules ref
    
    srand(time(0)); //set randoms seed
    InitAudioDevice();

    removeAutoSFX =  LoadSound("Resources/Audio/removeAuto.wav");
    resetSFX = LoadSound("Resources/Audio/reset.wav");
    placeAutoSFX = LoadSound("Resources/Audio/placeAuto.wav");
}

SandStorm::~SandStorm() //deconstructor
{
    delete elementRules;
}

//Main update loop
void SandStorm::Update(float deltaTime)
{
    this->mousePosition = GetMousePosition();
    
    HandleCellSwitching();
    HandleInput((int)mousePosition.x, (int)mousePosition.y);

    //Try update all active cells
    if (shouldUpdate)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT - 2; y++)
            {
                UpdateCell(x, y);
            }
        }
    }
    UpdateTexture(screenTexture, pixels); //NOTE: does texture need to be updated every frame?
}

//Main render loop
void SandStorm::Render()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexture(screenTexture, 0, 0, WHITE);
    //draw custom cursor
    Vector2 cursorPosition = Vector2((int)mousePosition.x - cursorOrigin, (int)mousePosition.y - cursorOrigin);
    DrawTexture(cursor, cursorPosition.x, cursorPosition.y, WHITE);

    if (showHudInfo) 
    {
        DrawFPS(0, 0); //draw fps
        DrawText(GetElementString().c_str(), 0, 24, 24, GREEN); //draw current element and brush size
        DrawText(shouldUpdate ? "Active" : "Paused", 256 - 45, 0, 24, GREEN); //draw update state label
    }

    //Update auto cell manipulators at end of frame
    for (const auto& manipulator : SandStorm::autoManipulators)
    {
        float scale = manipulator.position.z * 2;
        float xPos = manipulator.position.x;
        float yPos = manipulator.position.y;

        if(showHudInfo)
            DrawRectangleLines(xPos - manipulator.position.z, yPos - manipulator.position.z, scale, scale, manipulator.mode ? GREEN : RED);
        if(shouldUpdate)
            ManipulateCell(manipulator.mode, xPos, yPos, manipulator.placeElement, manipulator.position.z);
    }

    EndDrawing();
    if (skipTimerActive)
        shouldUpdate = false;
}

//Update cell based on its rules
void SandStorm::UpdateCell(int x, int y)
{
    int oldIndex = x + WIDTH * y;
    int currentCell = map[oldIndex].type;
    
    if (currentCell == 0 || currentCell == 3) //skip air (empty)/wall cells
        return;

    if (map[oldIndex].isUpdated) //skip cell if it has already beed updated this frame
    {
        map[oldIndex].isUpdated = false;
        return;
    }

    Element::Elements currentElement = static_cast<Element::Elements>(currentCell);
    auto& cellRuleSet = elementRules->getRuleSet[currentElement]; //get the right ruleset based on cell element type

    for (const auto& rule : cellRuleSet) //loop through all rules of the ruleset
    {
        Vector2 checkVector = elementRules->ruleValues[rule];
        int xPos = checkVector.x;
        int yPos = checkVector.y;

        if (IsOutOfBounds(xPos + x, yPos + y)) //check if next desired position is out of bounds
            continue;

        //try to go to desired postion based on current rule
        int newIndex = (x + xPos) + WIDTH * (y + yPos);
        if (map[newIndex].type == 0) 
        {
            SetCell(oldIndex, Element::Elements::UNOCCUPIED, false);
            SetCell(newIndex, currentElement);
            break;
        }
        
        #pragma region CellInteractions
        //swap sand with water if sand falls on top of water
        if (currentCell == 1 && map[newIndex].type == 2)
        {
            SwapCell(oldIndex, newIndex, Element::Elements::SAND, Element::Elements::WATER);
            break;
        }

        //create smoke when water or sand touches 
        bool createObsidianFromSand = currentCell == 1 && map[newIndex].type == 5;
        bool createObsidianFromWater = currentCell == 2 && map[newIndex].type == 5;

        if (createObsidianFromSand || createObsidianFromWater)
        {
            SetCell(oldIndex, Element::Elements::SMOKE);
            SetCell(newIndex, Element::Elements::OBSIDIAN);
            break;
        }

        if (currentCell == 5 && map[newIndex].type == 1) //create obsidian when lava touches sand
            SetCell(newIndex, Element::Elements::OBSIDIAN);

        if (currentCell == 5 && map[newIndex].type == 7) //create fire when lava touches wood
            SetCell(newIndex, Element::Elements::FIRE);

        //TODDO: Working on fire behaviour needs lots of attention
        if (currentCell == 7) {
            int upIndex = (x)+WIDTH * (y - 1);
            if (map[upIndex].type == 8)
                SetCell(oldIndex, Element::Elements::FIRE);
        }
        #pragma endregion
    }
}

//Helper method for setting single cells
void SandStorm::SetCell(int index, Element::Elements element, bool markUpdated)
{
    pixels[index] = elementRules->GetCellColor(element);
    map[index].type = element;
    map[index].isUpdated = markUpdated;
}

//Helper method for swapping two cells with each other
void SandStorm::SwapCell(int fromIndex, int toIndex, Element::Elements swapA, Element::Elements swapB)
{
    pixels[fromIndex] = elementRules->GetCellColor(swapB);
    pixels[toIndex] = elementRules->GetCellColor(swapA);
    
    map[fromIndex].type = swapB;
    map[toIndex].type = swapA;

    map[fromIndex].isUpdated = true;
    map[toIndex].isUpdated = true;
}

//Placing / destroying cells with mouse
void SandStorm::ManipulateCell(bool state, int xPos, int yPos, Element::Elements placeElement, int overrideBrushSize)
{
    int brushSize = overrideBrushSize == 0 ? this->brushSize : overrideBrushSize;
    for (int x = -brushSize; x < brushSize; x++)
    {
        for (int y = -brushSize; y < brushSize; y++)
        {
            int index = (x + xPos) + WIDTH * (y + yPos);

            if (IsOutOfBounds(xPos + x, yPos + y)) //check for all brush positions if it is out of bounds
                continue;
            
            if (state) //placing cells 
            {
                //placing fill chance based on current element and brush size
                float fillChance = (placeElement == Element::Elements::WALL || placeElement == Element::Elements::WOOD) ? cellPlacingNoRandomization : cellPlacingRandomization;
                if (GetChance(fillChance))
                {
                    if (map[index].type == 0)
                        SetCell(index, placeElement, false);
                }
            }
            else //destroying cells
            {
                if (map[index].type > 0)
                    SetCell(index, Element::UNOCCUPIED, false);
            }
        }
    }
}

//General input checks
void SandStorm::HandleInput(int mouseX, int mouseY)
{
    if (IsMouseButtonDown(0)) //placing cells
        ManipulateCell(true, mouseX, mouseY, currentElement);

    if (IsMouseButtonDown(1)) //removing cells
        ManipulateCell(false, mouseX, mouseY, currentElement);

    if (IsKeyPressed(KEY_LEFT_BRACKET)) //increase brush size
    {
        brushSize -= IsKeyDown(KEY_LEFT_CONTROL) ? brushSizeScaler : 1;
        brushSize = std::max(brushSize, 1);
    }

    if (IsKeyPressed(KEY_RIGHT_BRACKET)) //decrease brush size
        brushSize += IsKeyDown(KEY_LEFT_CONTROL) ? brushSizeScaler : 1;

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) //make screenshot
        ExportScreenShot();

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(0)) //create auto placer
    {
        PlaySound(placeAutoSFX);
        autoManipulators.push_back(AutoCellManipulator(mousePosition, brushSize, true, currentElement));
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(1)) //create auto destroyer
    {
        PlaySound(placeAutoSFX);
        autoManipulators.push_back(AutoCellManipulator(mousePosition, brushSize, false));
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) //undo last auto cell manipulator
    {
        if (autoManipulators.size() == 0)
            return;

        PlaySound(removeAutoSFX);
        autoManipulators.pop_back();
    }

    if (IsKeyPressed(KEY_TAB)) //reset sim
    {
        PlaySound(resetSFX);
        autoManipulators.clear();
        ManipulateCell(false, screenCenter.x, screenCenter.y, Element::Elements::UNOCCUPIED, 256);
    }

    if (IsKeyPressed(KEY_GRAVE)) //toggle ui/debug info
        showHudInfo = !showHudInfo;

    if (IsKeyPressed(KEY_SPACE)) //toggle updating
    {
        shouldUpdate = !shouldUpdate;
        skipTimerActive = !shouldUpdate;
    }

    if (IsKeyPressed(KEY_RIGHT)) //go couple frames forward
    {
        shouldUpdate = true;
        skipTimerActive = true;
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
        currentElement = Element::Elements::SMOKE;

    if (IsKeyPressed(KEY_FIVE))
        currentElement = Element::Elements::LAVA;
    
    if (IsKeyPressed(KEY_SIX))
        currentElement = Element::Elements::WOOD;
}

//Helper method for creating and exporting screenshots
void SandStorm::ExportScreenShot()
{
    std::filesystem::path directoryPath = GetApplicationDirectory(); //define screenshot path
    directoryPath /= "Screenshots";

    if (!std::filesystem::exists(directoryPath)) //create 'Screenshot' folder if it doesn't exist
        std::filesystem::create_directory(directoryPath);

    //gather timestamp info
    struct tm timeInfo;
    auto currentTime = std::chrono::system_clock::now();
    auto currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
    localtime_s(&timeInfo, &currentTimeT);

    strftime(timeBuffer, sizeof(timeBuffer), "%d-%H-%M-%S-", &timeInfo); //format timestamp based on timeInfo and put it inside timeBuffer

    std::string timeStamp(timeBuffer);
    std::filesystem::path imagePath = directoryPath / (timeStamp + "screenshot.png"); //define file name/path

    //load and export current screen texture 
    Image image = LoadImageFromTexture(screenTexture);
    ExportImage(image, imagePath.string().c_str());
    
    UnloadImage(image);
}

//Checks if given position is outside the window
bool SandStorm::IsOutOfBounds(int posX, int posY)
{
    bool outOfBoundsA = posX >= WIDTH || posY > HEIGHT;
    bool outOfBoundsB = posX < 0 || posY < 0;

    return outOfBoundsA || outOfBoundsB;
}

//Calculates and returns a chance based on input value
bool SandStorm::GetChance(float input)
{
    return GetRandomValue(0, 100) > input;
}

//Convert current element enum value to string for UI label
std::string SandStorm::GetElementString()
{
    switch (currentElement)
    {
        case 1:  return "Sand " +      std::to_string(brushSize);
        case 2:  return "Water " +     std::to_string(brushSize);
        case 3:  return "Wall " +      std::to_string(brushSize);
        case 4:  return "Smoke " +     std::to_string(brushSize);
        case 5:  return "Lava " +      std::to_string(brushSize);
        case 7:  return "Wood " +      std::to_string(brushSize);
        default: return "UNDIFINED " + std::to_string(brushSize);
    }
}
