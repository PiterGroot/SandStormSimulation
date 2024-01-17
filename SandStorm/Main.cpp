#include "SandStorm.h"

constexpr auto SCREEN_WIDTH = 512;
constexpr auto SCREEN_HEIGHT = 512;

int main()
{
    SandStorm* sandStorm = new SandStorm();
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib goo brrrrrrrrrrr"); //create raylib window
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime(); //calculate deltaTime
        sandStorm->Update(deltaTime); //update cell engine
    }

    CloseWindow();
    return 0;
}