#include "InputHandler.h"
#include "SandStorm.h"

InputHandler::InputHandler(Vector2 screenCenter)
{
    this->screenCenter = screenCenter;
}

void InputHandler::OnUpdate(Vector2 mousePosition)
{
    SandStorm::instance->imageImporter->OnUpdate();

    if (IsMouseButtonDown(0)) //placing cells
        SandStorm::instance->ManipulateCell(true, mousePosition.x, mousePosition.y, SandStorm::instance->currentElement);

    if (IsMouseButtonDown(1)) //removing cells
        SandStorm::instance->ManipulateCell(false, mousePosition.x, mousePosition.y, SandStorm::instance->currentElement);

    if (IsKeyPressed(KEY_LEFT_BRACKET)) //increase brush size
    {
        SandStorm::instance->brushSize -= IsKeyDown(KEY_LEFT_CONTROL) ? SandStorm::instance->brushSizeScaler : 1;
        SandStorm::instance->brushSize = std::max(SandStorm::instance->brushSize, 1);
    }

    if (IsKeyPressed(KEY_RIGHT_BRACKET)) //decrease brush size
        SandStorm::instance->brushSize += IsKeyDown(KEY_LEFT_CONTROL) ? SandStorm::instance->brushSizeScaler : 1;

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) //make screenshot
        SandStorm::instance->ExportScreenShot();

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(0)) //create auto placer
    {
        PlaySound(SandStorm::instance->placeAutoSFX);
        SandStorm::instance->autoManipulators.push_back(SandStorm::AutoCellManipulator(mousePosition, SandStorm::instance->brushSize, true, SandStorm::instance->currentElement));
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(1)) //create auto destroyer
    {
        PlaySound(SandStorm::instance->placeAutoSFX);
        SandStorm::instance->autoManipulators.push_back(SandStorm::AutoCellManipulator(mousePosition, SandStorm::instance->brushSize, false));
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) //undo last auto cell manipulator
    {
        if (SandStorm::instance->autoManipulators.size() == 0)
            return;

        PlaySound(SandStorm::instance->removeAutoSFX);
        SandStorm::instance->autoManipulators.pop_back();
    }

    if (IsKeyPressed(KEY_TAB)) //reset sim
        SandStorm::instance->ResetSim();

    if (IsKeyPressed(KEY_GRAVE)) //toggle ui/debug info
        SandStorm::instance->showHudInfo = !SandStorm::instance->showHudInfo;

    if (IsKeyPressed(KEY_SPACE)) //toggle updating
    {
        SandStorm::instance->shouldUpdate = !SandStorm::instance->shouldUpdate;
        SandStorm::instance->skipTimerActive = !SandStorm::instance->shouldUpdate;
    }

    if (IsKeyPressed(KEY_RIGHT)) //go couple frames forward
    {
        SandStorm::instance->shouldUpdate = true;
        SandStorm::instance->skipTimerActive = true;
    }
}