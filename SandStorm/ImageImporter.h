#pragma once

#include <vector>
#include <string>

class ImageImporter
{
public:
	ImageImporter(int screenWidth);
	~ImageImporter();

	void ImportImage(int imageIndex);
	void OnUpdate();

private:
	std::vector<std::string> imageNames;
	int screenWidth = 0;
};

