#pragma once
#include "IDataSet.h"
#include "ImageDataParams.h"

class ImageDataSet :
	public IDataSet
{
	protected:
		int x, y, sizeX, sizeY;
		std::string filepath;
		int frameCounter;

	public:
		ImageDataSet(int x, int y, int sizeX, int sizeY, std::string filepath);
		~ImageDataSet(void);
		void notifyRender();
		bool notifyUpdate(int frameNumber);
};

