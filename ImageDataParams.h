#pragma once
#include "BaseDataParams.h"
#include <string>
// Deprecated

class ImageDataParams :
	public BaseDataParams
{
	public:
		int x, y, xSize, ySize, frameRate;
		std::string imagePath;
};