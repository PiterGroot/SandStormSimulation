#include "Cell.h"
#include "raylib.h"

Color yellow = Color(255, 255, 0, 255);

void Cell::Draw(int x, int y)
{
    DrawPixel(x, y, yellow); //draw cell at given position
}

