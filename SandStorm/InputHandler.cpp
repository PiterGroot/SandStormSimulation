#include "InputHandler.h"

InputHandler::InputHandler(SandStorm* engine, Vector2 screenCenter)
{
    this->engine = engine;
    this->screenCenter = screenCenter;
}

void InputHandler::OnUpdate(Vector2 mousePosition)
{
    if (IsMouseButtonDown(0)) //placing cells
        engine->ManipulateCell(true, mousePosition.x, mousePosition.y, engine->currentElement);

    if (IsMouseButtonDown(1)) //removing cells
        engine->ManipulateCell(false, mousePosition.x, mousePosition.y, engine->currentElement);

    if (IsKeyPressed(KEY_LEFT_BRACKET)) //increase brush size
    {
        engine->brushSize -= IsKeyDown(KEY_LEFT_CONTROL) ? engine->brushSizeScaler : 1;
        engine->brushSize = std::max(engine->brushSize, 1);
    }

    if (IsKeyPressed(KEY_RIGHT_BRACKET)) //decrease brush size
        engine->brushSize += IsKeyDown(KEY_LEFT_CONTROL) ? engine->brushSizeScaler : 1;

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) //make screenshot
        engine->ExportScreenShot();

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(0)) //create auto placer
    {
        PlaySound(engine->placeAutoSFX);
        engine->autoManipulators.push_back(SandStorm::AutoCellManipulator(mousePosition, engine->brushSize, true, engine->currentElement));
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(1)) //create auto destroyer
    {
        PlaySound(engine->placeAutoSFX);
        engine->autoManipulators.push_back(SandStorm::AutoCellManipulator(mousePosition, engine->brushSize, false));
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) //undo last auto cell manipulator
    {
        if (engine->autoManipulators.size() == 0)
            return;

        PlaySound(engine->removeAutoSFX);
        engine->autoManipulators.pop_back();
    }

    if (IsKeyPressed(KEY_TAB)) //reset sim
    {
        PlaySound(engine->resetSFX);
        engine->autoManipulators.clear();
        engine->ManipulateCell(false, screenCenter.x, screenCenter.y, Element::Elements::UNOCCUPIED, 256);
    }

    if (IsKeyPressed(KEY_GRAVE)) //toggle ui/debug info
        engine->showHudInfo = !engine->showHudInfo;

    if (IsKeyPressed(KEY_SPACE)) //toggle updating
    {
        engine->shouldUpdate = !engine->shouldUpdate;
        engine->skipTimerActive = !engine->shouldUpdate;
    }

    if (IsKeyPressed(KEY_RIGHT)) //go couple frames forward
    {
        engine->shouldUpdate = true;
        engine->skipTimerActive = true;
    }
}
