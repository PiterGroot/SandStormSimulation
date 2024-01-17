#include "SandStorm.h"
#include "Cell.h"

constexpr auto WIDTH = 512;
constexpr auto HEIGHT = 512;

//Cell* cells[WIDTH][HEIGHT];
std::map<Vector2, Cell*> cells;

SandStorm::SandStorm() {}

void SandStorm::Update(float deltaTime)
{
    BeginDrawing();
    ClearBackground(BLACK);

    HandlePlacingCell();
    for (int x = 0; x < WIDTH; x++) //loop over all cells
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            if (cells[Vector2(x, y)] == nullptr)
                continue;

            if (cells[Vector2(x, y)]->element == 0)
            {
                cells[Vector2(x, y)]->Update(this);
                cells[Vector2(x, y)]->Draw();
            }
        }
    }

    DrawFPS(0, 0); //draw current fps
    EndDrawing();
}

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

        if (cells[Vector2(xPos, yPos)] != nullptr) 
        {
            if (cells[Vector2(xPos, yPos)]->element == 1) 
            {
                std::cout << "eeioeioe" << "\n";
                return;
            }
        }
        cells[Vector2(xPos, yPos)] = new Cell(xPos, yPos);
    }
}