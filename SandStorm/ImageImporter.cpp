#include "ImageImporter.h"
#include "SandStorm.h"

ImageImporter::ImageImporter(int screenWidth)
{
    this->screenWidth = screenWidth;

	imageNames.push_back("Textures/img.png");
	imageNames.push_back("Textures/img1.png");
	imageNames.push_back("Textures/img2.png");
	imageNames.push_back("Textures/img3.png");
    imageNames.push_back("Textures/img4.png");
}

ImageImporter::~ImageImporter()
{
	imageNames.clear();
}

//Clear sim grid and import/place image pixels
void ImageImporter::ImportImage(int imageIndex)
{
    SandStorm::instance->ResetSim();
    Image image = LoadImage(imageNames[imageIndex].c_str());
    Color* imagePixels = LoadImageColors(image);

    int blackPixelCount = 0;
    for (size_t x = 0; x < image.width; x++)
    {
        for (int y = 0; y < image.height; y++)
        {
            int index = x + image.width * y;
            if (imagePixels[index].a > 0)
                SandStorm::instance->SetCell(x + screenWidth * y, Element::WOOD);
        }
    }

    UnloadImageColors(imagePixels);
    UnloadImage(image);
}

void ImageImporter::OnUpdate()
{
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_ONE))   ImportImage(0);
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_TWO))   ImportImage(1);
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_THREE)) ImportImage(2);
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_FOUR))  ImportImage(3);
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_FIVE))  ImportImage(4);
}
