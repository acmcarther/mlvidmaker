#pragma once
#include "IDataSet.h"
#include "ImageDataParams.h"
#include <sstream>

class ImageDataSet :
	public IDataSet
{
	protected:
		int x, y, sizeX, sizeY;
		std::string filepath;
		std::string filetype;
		SDL_Surface* currentImage;
		int frameCounter;

	public:
		ImageDataSet(int x, int y, int sizeX, int sizeY, std::string filepath, int firstImageId, std::string filetype);
		~ImageDataSet(void);
		void notifyRender(SDL_Surface* screenSurface);
		bool notifyUpdate(int frameNumber);
};

