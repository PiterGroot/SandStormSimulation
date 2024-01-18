#include "Cell.h"
#include "raylib.h"

void Cell::Draw(int x, int y)
{
    DrawPixel(x, y, cellColor); //draw cell at given position
}

