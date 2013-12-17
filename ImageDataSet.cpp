#include "ImageDataSet.h"
#include <iostream>
#include <fstream>
#include <SDL_image.h>
#include <sys/types.h>
#include <sys/stat.h>


ImageDataSet::ImageDataSet(int x, int y, int sizeX, int sizeY, std::string filepath, int firstImageId, std::string filetype)
{
	this->x = x;
	this->y = y;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->filepath = filepath;
	this->filetype = filetype;
	this->frameCounter = firstImageId;
	this->currentImage = nullptr;
}


ImageDataSet::~ImageDataSet(void)
{
}

void ImageDataSet::notifyRender(SDL_Surface* screenSurface)
{
    // Make a temporary rectangle to hold the offsets
    SDL_Rect offset;

    // Give the offsets to the rectangle
    offset.x = this->x;
    offset.y = this->y;
	offset.h = this->sizeY;
	offset.w = this->sizeX;

    // Blit the surface
    SDL_BlitSurface( currentImage, NULL, screenSurface, &offset );
}

bool ImageDataSet::notifyUpdate(int frameNumber)
{
	// Define some handy variables
	SDL_Surface* loadedImage = nullptr;
	SDL_Surface* optimizedImage = nullptr;
	std::ostringstream filenameBuffer;
	std::fstream fin;

	// Clear out the old image
	if( currentImage != nullptr )
	{
		SDL_FreeSurface(currentImage);
		currentImage = nullptr;
	}


	// Construct the filename
	filenameBuffer << filepath << "/" << (frameCounter+frameNumber) << "." << filetype;

	// Try to load the image of the frame number
	loadedImage = IMG_Load(filenameBuffer.str().c_str());

	// Check if we have a frame we can load into the buffer
	if (!loadedImage)
	{
		// We failed, either out of bounds (expected), or malformed dataset (not)
		std::cout << "WARN: " << filenameBuffer.str() << " was not a valid file. Closing data set. " << std::endl;
		std::cout << SDL_GetError() <<std::endl;

		// Notify load fail
		return false;
	}

	// Optimize the image
	currentImage = loadedImage;

	// Print the file modification time for debug
	struct stat buf;
	stat(filenameBuffer.str().c_str(), &buf);
	std::cout << buf.st_mtime << std::endl;
	
	// Notify load success
	return true;
}
