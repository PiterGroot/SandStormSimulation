#include "ImageImporter.h"
#include "SandStorm.h"

ImageImporter::ImageImporter(int screenWidth)
{
    this->screenWidth = screenWidth;

    //std::filesystem::path directoryPath = GetApplicationDirectory(); //define Images path
    //directoryPath /= "Textures\\Images";

    //if (!std::filesystem::exists(directoryPath)) //create 'Images' folder if it doesn't exist
    //{
    //    std::filesystem::create_directory(directoryPath);
    //    return;
    //}

    //for (const auto& possibleImage : std::filesystem::directory_iterator(directoryPath))
    //{
    //    if (possibleImage.path().extension() == ".png") //check if file is a png
    //    {
    //        maxImagesCount++;
    //        imageNames.push_back("Textures/Images/" + possibleImage.path().filename().string());
    //    }
    //}
}

ImageImporter::~ImageImporter()
{
	imageNames.clear();
}

//Clear sim grid and import/place image pixels
void ImageImporter::ImportImage(int imageIndex)
{
    SandStorm::instance->ResetSim();
    currentImportedImage = imageNames[imageIndex].c_str();

    Image image = LoadImage(imageNames[imageIndex].c_str());
    Color* imagePixels = LoadImageColors(image);

    int blackPixelCount = 0;
    for (size_t x = 0; x < image.width; x++)
    {
        for (int y = 0; y < image.height; y++)
        {
            int index = x + image.width * y;
            if (imagePixels[index].a > 0) {
                
                Element::Elements cellElement = GetCellElement(imagePixels[index]);
                SandStorm::instance->SetCell(x + screenWidth * y, cellElement);
            }
        }
    }

    UnloadImageColors(imagePixels);
    UnloadImage(image);
}

//Returns correct cell element based on raw pixel color
Element::Elements ImageImporter::GetCellElement(Color rawPixelColor)
{
    if (CompareColor(rawPixelColor, Color(255, 255, 0, 255)))   return Element::Elements::SAND;
    if (CompareColor(rawPixelColor, Color(0, 0, 255, 255)))     return Element::Elements::WATER;
    if (CompareColor(rawPixelColor, Color(255, 255, 255, 255))) return Element::Elements::WALL;
    if (CompareColor(rawPixelColor, Color(128, 128, 128, 255))) return Element::Elements::SMOKE;
    if (CompareColor(rawPixelColor, Color(255, 0, 0, 255)))     return Element::Elements::LAVA;
    if (CompareColor(rawPixelColor, Color(0, 0, 0, 255)))       return Element::Elements::OBSIDIAN;
    if (CompareColor(rawPixelColor, Color(127, 51, 0, 255)))    return Element::Elements::WOOD;
    if (CompareColor(rawPixelColor, Color(255, 106, 0, 255)))   return Element::Elements::FIRE;
}

//Helper method for comparing colors
bool ImageImporter::CompareColor(Color colorA, Color colorB)
{
    bool r = colorA.r == colorB.r;
    bool g = colorA.g == colorB.g;
    bool b = colorA.b == colorB.b;
    bool a = colorA.a == colorB.a;

    return r && g && b && a;
}

//Shortcuts for easily importing images
void ImageImporter::OnUpdate()
{
    if (IsKeyDown(KEY_LEFT_CONTROL)) 
    {
        float wheel = GetMouseWheelMove();
        if (wheel == 0)
            return;

        currentImage += wheel;
        currentImage = std::clamp(currentImage, 0, maxImagesCount - 1);
      
        ImportImage(currentImage);
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_ONE))   ImportImage(0);
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_TWO))   ImportImage(1);
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_THREE)) ImportImage(2);
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_FOUR))  ImportImage(3);
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_FIVE))  ImportImage(4);
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_SIX))   ImportImage(5);
}