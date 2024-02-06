#pragma once

#include "raylib.h"
#include <vector>
#include <string>
#include "Element.h"

class ImageImporter
{
public:
    ImageImporter(int screenWidth);
    ~ImageImporter();

    void ImportImage(int imageIndex);
    void OnUpdate();

    std::string currentImportedImage;

private:
    Element::Elements GetCellElement(Color rawPixelColor);
    bool CompareColor(Color colorA, Color colorB);\

    int screenWidth = 0;
    int currentImage = 0;
    int maxImagesCount = 0;
    
    std::vector<std::string> imageNames;
};
