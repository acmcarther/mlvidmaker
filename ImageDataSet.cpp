#include "ImageDataSet.h"
#include <iostream>

ImageDataSet::ImageDataSet(int x, int y, int sizeX, int sizeY, std::string filepath)
{
	this->x = x;
	this->y = y;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->filepath = filepath;
	this->frameCounter = 0;
}


ImageDataSet::~ImageDataSet(void)
{
}

void ImageDataSet::notifyRender()
{

}

bool ImageDataSet::notifyUpdate(int frameNumber)
{
	std::cout << "Updating: " << x << ", " << y << ", " << filepath << std::endl;
	frameCounter++;
	return true;
}
