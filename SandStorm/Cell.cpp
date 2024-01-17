#include "Cell.h"

Vector3 yellow = Vector3(255, 255, 0);

Cell::Cell(int x, int y)
{
    cellPosition = Vector2(x, y);
}

void Cell::Update(SandStorm* engine)
{
    Vector2 newPosition = Vector2(cellPosition.x, cellPosition.y + 1);
    cellPosition = newPosition;
}

void Cell::Draw()
{
    DrawPixelV(cellPosition, Color(yellow.x, yellow.y, yellow.z, 255)); //draw cell at current position
}

